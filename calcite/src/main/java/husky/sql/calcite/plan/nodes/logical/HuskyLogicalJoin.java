package husky.sql.calcite.plan.nodes.logical;

import org.apache.calcite.plan.RelOptCluster;
import org.apache.calcite.plan.RelOptCost;
import org.apache.calcite.plan.RelOptPlanner;
import org.apache.calcite.plan.RelTraitSet;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.core.Join;
import org.apache.calcite.rel.core.JoinRelType;
import org.apache.calcite.rel.core.CorrelationId;
import org.apache.calcite.rel.metadata.RelMetadataQuery;
import org.apache.calcite.rex.RexLiteral;
import org.apache.calcite.rex.RexNode;

import java.util.List;
import java.util.HashSet;

public class HuskyLogicalJoin extends Join implements HuskyLogicalRel {
    public HuskyLogicalJoin(RelOptCluster cluster, RelTraitSet traitSet, RelNode left, RelNode right, RexNode condition, JoinRelType joinType) {
        super(cluster, traitSet, left, right, condition, new HashSet<CorrelationId>(), joinType);
    }

    @Override
    public Join copy(RelTraitSet traitSet, RexNode conditionExpr, RelNode left, RelNode right, JoinRelType joinType, boolean semiJoinDone) {
        return new HuskyLogicalJoin(getCluster(), traitSet, left, right, conditionExpr, joinType);
    }

    @Override
    public RelOptCost computeSelfCost(RelOptPlanner planner, RelMetadataQuery mq) {
    	double leftRowCnt = mq.getRowCount(this.getLeft());
        double leftRowSize = this.estimateRowSize(this.getLeft().getRowType());

        double rightRowCnt = mq.getRowCount(this.getRight());
        double rightRowSize = this.estimateRowSize(this.getRight().getRowType());

        double ioCost = (leftRowCnt * leftRowSize) + (rightRowCnt * rightRowSize);
        double cpuCost = leftRowCnt + rightRowCnt;
        double rowCnt = leftRowCnt + rightRowCnt;

    	return planner.getCostFactory().makeCost(rowCnt, cpuCost, ioCost);
    }
}
