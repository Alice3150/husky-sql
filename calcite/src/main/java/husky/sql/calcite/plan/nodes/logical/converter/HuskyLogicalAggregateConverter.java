package husky.sql.calcite.plan.nodes.logical.converter;

import husky.sql.calcite.plan.nodes.HuskyConventions;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalAggregate;
import org.apache.calcite.plan.Convention;
import org.apache.calcite.plan.RelOptRule;
import org.apache.calcite.plan.RelTraitSet;
import org.apache.calcite.plan.RelOptRuleCall;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.convert.ConverterRule;
import org.apache.calcite.rel.logical.LogicalAggregate;
import org.apache.calcite.rel.core.AggregateCall;
import org.apache.calcite.sql.SqlKind;

import java.util.List;
import java.util.stream.Collectors;

public class HuskyLogicalAggregateConverter extends ConverterRule {

    public static HuskyLogicalAggregateConverter INSTANCE = new HuskyLogicalAggregateConverter();

    public HuskyLogicalAggregateConverter() {
        super(LogicalAggregate.class, Convention.NONE, HuskyConventions.LOGICAL, "HuskyLogicalAggregateConverter");
    }

    @Override
    public boolean matches(RelOptRuleCall call){
        LogicalAggregate agg = call.rel(0);
        // we do not support these functions natively
        // they have to be converted using the AggregateReduceFunctionsRule
        List<AggregateCall> aggCallList = agg.getAggCallList();
        List<SqlKind> kindList = aggCallList.stream()
                                .map(elt -> elt.getAggregation().getKind())
                                .collect(Collectors.toList());
        for(SqlKind kind : kindList) {
            if(kind != SqlKind.AVG && SqlKind.AVG_AGG_FUNCTIONS.contains(kind)) {
                return false;
            } 
        }
        return true;
    }

    @Override
    public RelNode convert(RelNode rel) {
        LogicalAggregate agg = (LogicalAggregate)rel;
        RelTraitSet traitSet = rel.getTraitSet().replace(HuskyConventions.LOGICAL);
        RelNode newInput = RelOptRule.convert(agg.getInput(), HuskyConventions.LOGICAL);

        return new HuskyLogicalAggregate(rel.getCluster(), traitSet, newInput, agg.indicator, agg.getGroupSet(), agg.getGroupSets(), agg.getAggCallList());
    }
}