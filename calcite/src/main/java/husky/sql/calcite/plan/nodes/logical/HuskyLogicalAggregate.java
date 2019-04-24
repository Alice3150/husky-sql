package husky.sql.calcite.plan.nodes.logical;

import org.apache.calcite.plan.RelOptCluster;
import org.apache.calcite.plan.RelOptCost;
import org.apache.calcite.plan.RelOptPlanner;
import org.apache.calcite.plan.RelTraitSet;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.RelCollation;
import org.apache.calcite.rel.core.Aggregate;
import org.apache.calcite.rel.core.AggregateCall;
import org.apache.calcite.rel.metadata.RelMetadataQuery;
import org.apache.calcite.rex.RexLiteral;
import org.apache.calcite.rex.RexNode;
import org.apache.calcite.util.ImmutableBitSet;

import java.util.List;

public class HuskyLogicalAggregate extends Aggregate implements HuskyLogicalRel {
    public HuskyLogicalAggregate(RelOptCluster cluster, RelTraitSet traitSet, RelNode child, boolean indicator, ImmutableBitSet groupSet, List<ImmutableBitSet> groupSets, List<AggregateCall> aggCalls) {
        super(cluster, traitSet, child, indicator, groupSet, groupSets, aggCalls);
    }

    @Override
    public Aggregate copy(RelTraitSet traitSet, RelNode input, boolean indicator, ImmutableBitSet groupSet, List<ImmutableBitSet> groupSets, List<AggregateCall> aggCalls) {
        return new HuskyLogicalAggregate(getCluster(), traitSet, input, indicator, groupSet, groupSets, aggCalls);
    }

    @Override
    public RelOptCost computeSelfCost(RelOptPlanner planner, RelMetadataQuery mq) {
    	RelNode child = this.getInput();
    	double rowCnt = mq.getRowCount(child);
        double rowSize = this.estimateRowSize(child.getRowType());
        int aggCnt = this.getAggCallList().size();
    	return planner.getCostFactory().makeCost(rowCnt, rowCnt * aggCnt, rowCnt * rowSize);
    }
}
