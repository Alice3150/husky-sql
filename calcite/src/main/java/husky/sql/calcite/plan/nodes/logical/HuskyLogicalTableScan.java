package husky.sql.calcite.plan.nodes.logical;

import husky.sql.calcite.plan.rules.logical.PushProjectIntoTableScanRule;
import husky.sql.calcite.plan.util.*;

import org.apache.calcite.plan.RelOptCluster;
import org.apache.calcite.plan.RelOptCost;
import org.apache.calcite.plan.RelOptPlanner;
import org.apache.calcite.plan.RelOptTable;
import org.apache.calcite.plan.RelTraitSet;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.RelWriter;
import org.apache.calcite.rel.core.TableScan;
import org.apache.calcite.rel.metadata.RelMetadataQuery;
import org.apache.calcite.rel.type.RelDataType;
import org.apache.calcite.rel.type.RelDataTypeFactory;
import org.apache.calcite.rel.type.RelDataTypeField;
import org.apache.calcite.rel.metadata.RelMdUtil;
import org.apache.calcite.rex.RexNode;
import org.apache.calcite.rex.RexLocalRef;
import org.apache.calcite.rex.RexCall;

import java.util.List;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Arrays;
import java.util.stream.Collectors;

public class HuskyLogicalTableScan extends TableScan implements HuskyLogicalRel {
    public int[] fields;
    public RexCall condition;
    private boolean isFilterPushDown;

    public HuskyLogicalTableScan(RelOptCluster cluster, RelTraitSet traitSet, RelOptTable table, int[] selectedFields) {
        super(cluster, traitSet, table);
        this.fields = Arrays.copyOf(selectedFields, selectedFields.length);
        this.isFilterPushDown = false;
    }

    @Override
    public RelWriter explainTerms(RelWriter pw) {
        List<String> allFieldList = table.getRowType().getFieldNames();
        List<String> selectedFieldList = new ArrayList<String>();
        for(int i = 0; i < fields.length; i++) {
            selectedFieldList.add(allFieldList.get(fields[i]) + "=[$" + fields[i] + "]");
        }

        if(this.isFilterPushDown) {
            return super.explainTerms(pw)
                        .item("fields", selectedFieldList.stream().collect(Collectors.joining(", ")))
                        .item("condition", condition);
        } else {
            return super.explainTerms(pw)
                    .item("fields", selectedFieldList.stream().collect(Collectors.joining(", ")));
        }   
    }

    @Override
    public RelDataType deriveRowType() {
        final List<RelDataTypeField> fieldList = table.getRowType().getFieldList();
        final RelDataTypeFactory.Builder builder = getCluster().getTypeFactory().builder();
        if(fields.length == 0) {
            // no project
            for(RelDataTypeField field : fieldList) {
                builder.add(field);
            }
        } else {
            for(int field : fields) {
                builder.add(fieldList.get(field));
            }
        }
        return builder.build();
    }

    // @Override
    // public void register(RelOptPlanner planner) {
    //     planner.addRule(PushProjectIntoTableScanRule.INSTANCE);
    // }

    @Override
    public RelOptCost computeSelfCost(RelOptPlanner planner, RelMetadataQuery mq) {
        // Multiply the cost by a factor that makes a scan more attractive if it
        // has significantly fewer fields than the original scan.
        //
        // The "+ 2D" on top and bottom keeps the function fairly smooth.
        //
        // For example, if table has 3 fields, project has 1 field,
        // then factor = (1 + 2) / (3 + 2) = 0.6
        double rowCnt = mq.getRowCount(this);
        if(this.fields.length != 0) {
            if(isFilterPushDown) {
                // with project and filter
                double selectivity = RelMdUtil.guessSelectivity(condition, false);
                double newRowCnt = Math.max(rowCnt * selectivity, 1.0);
                return planner.getCostFactory().makeCost(newRowCnt, newRowCnt, newRowCnt * estimateRowSize(getRowType())).multiplyBy(((double) fields.length + 2D)
                    / ((double) table.getRowType().getFieldCount() + 2D));
            } else {
                // with project but no filter
                return planner.getCostFactory().makeCost(rowCnt, rowCnt, rowCnt * estimateRowSize(getRowType())).multiplyBy(((double) fields.length + 2D)
                    / ((double) table.getRowType().getFieldCount() + 2D));
            }
        } else {
            if(isFilterPushDown) {
                // with filter and project all fields
                double selectivity = RelMdUtil.guessSelectivity(condition, false);
                double newRowCnt = Math.max(rowCnt * selectivity, 1.0);
                // System.out.println("--- cost with filter pushed down: " + planner.getCostFactory().makeCost(newRowCnt, newRowCnt, newRowCnt * estimateRowSize(getRowType())).toString() + ", Selectivity: " + selectivity + ", newRowCnt: " + newRowCnt);
                return planner.getCostFactory().makeCost(newRowCnt, newRowCnt, newRowCnt * estimateRowSize(getRowType()));
                // return super.computeSelfCost(planner, mq);
            } else {
                // project all fields and no filter
                // System.out.println("--- cost without filter pushed down: " + planner.getCostFactory().makeCost(rowCnt, rowCnt, rowCnt * estimateRowSize(getRowType())).toString());
                return planner.getCostFactory().makeCost(rowCnt, rowCnt, rowCnt * estimateRowSize(getRowType()));
                // return super.computeSelfCost(planner, mq);
            }
        }
    }

    // @Override
    public HuskyLogicalTableScan copy(RelTraitSet traitSet, int[] selectedFields) {
        // assert inputs.isEmpty();
        return new HuskyLogicalTableScan(getCluster(), getTraitSet(), getTable(), selectedFields);
    }

    public void applyPredicate(RexNode oldCondition, int[] usedFields) {
        // rewrite condition rexNode
        RexNode rewriteCondition = RexProgramRewriter.rewriteRexNode(oldCondition, usedFields);
        condition = (RexCall) rewriteCondition;
        this.isFilterPushDown = true;
    }

    public void applyPredicateByCopy(RexNode condition) {
        condition = (RexCall) condition;
        this.isFilterPushDown = true;
    }

    public boolean isFilterPushDown() {
        return this.isFilterPushDown;
    }
}
