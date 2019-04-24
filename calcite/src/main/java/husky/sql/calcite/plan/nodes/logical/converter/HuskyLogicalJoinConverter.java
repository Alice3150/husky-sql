package husky.sql.calcite.plan.nodes.logical.converter;

import husky.sql.calcite.plan.nodes.HuskyConventions;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalJoin;
import org.apache.calcite.plan.Convention;
import org.apache.calcite.plan.RelOptRule;
import org.apache.calcite.plan.RelTraitSet;
import org.apache.calcite.plan.RelOptRuleCall;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.plan.volcano.RelSubset;
import org.apache.calcite.rel.core.Project;
import org.apache.calcite.rel.core.Filter;
import org.apache.calcite.rel.core.Calc;
import org.apache.calcite.rel.core.Aggregate;
import org.apache.calcite.rel.logical.LogicalJoin;
import org.apache.calcite.rel.convert.ConverterRule;
import org.apache.calcite.rel.core.JoinRelType;
import org.apache.calcite.rel.core.JoinInfo;
import org.apache.calcite.sql.SqlKind;

import java.util.List;
import java.util.stream.Collectors;

public class HuskyLogicalJoinConverter extends ConverterRule {

    public static HuskyLogicalJoinConverter INSTANCE = new HuskyLogicalJoinConverter();

    public HuskyLogicalJoinConverter() {
        super(LogicalJoin.class, Convention.NONE, HuskyConventions.LOGICAL, "HuskyLogicalJoinConverter");
    }

    @Override
    public boolean matches(RelOptRuleCall call){
        LogicalJoin join = call.rel(0);
        JoinInfo joinInfo = join.analyzeCondition();

        return hasEqualityPredicates(joinInfo) || isSingleRowJoin(join);
    }

    @Override
    public RelNode convert(RelNode rel) {
        LogicalJoin join = (LogicalJoin)rel;
        RelTraitSet traitSet = rel.getTraitSet().replace(HuskyConventions.LOGICAL);
        RelNode newLeft = RelOptRule.convert(join.getLeft(), HuskyConventions.LOGICAL);
        RelNode newRight = RelOptRule.convert(join.getRight(), HuskyConventions.LOGICAL);

        return new HuskyLogicalJoin(rel.getCluster(), traitSet, newLeft, newRight, join.getCondition(), join.getJoinType());
    }

    private boolean hasEqualityPredicates(JoinInfo joinInfo) {
        return !joinInfo.pairs().isEmpty();
    }

    private boolean isSingleRowJoin(LogicalJoin join) {
        JoinRelType joinType = join.getJoinType();
        if(joinType == JoinRelType.INNER && (isSingleRow(join.getLeft()) || isSingleRow(join.getRight()))) {
            return true;
        } else if(joinType == JoinRelType.LEFT && isSingleRow(join.getRight())) {
            return true;
        } else if(joinType == JoinRelType.RIGHT && isSingleRow(join.getLeft())) {
            return true;
        } else {
            return false;
        }
    }

    private boolean isSingleRow(RelNode node) {
        if(node instanceof RelSubset) {
            RelSubset ss = (RelSubset)node;
            return isSingleRow(ss.getOriginal());
        } else if(node instanceof Project) {
            Project lp = (Project)node;
            return isSingleRow(lp.getInput());
        } else if(node instanceof Filter) {
            Filter lf = (Filter)node;
            return isSingleRow(lf.getInput());
        } else if(node instanceof Calc) {
            Calc lc = (Calc)node;
            return isSingleRow(lc.getInput());
        } else if(node instanceof Aggregate) {
            Aggregate la = (Aggregate)node;
            return la.getGroupSet().isEmpty();
        } else {
            return false;
        }
    }
}