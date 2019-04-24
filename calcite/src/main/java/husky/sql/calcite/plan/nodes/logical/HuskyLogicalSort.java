package husky.sql.calcite.plan.nodes.logical;

import org.apache.calcite.plan.RelOptCluster;
import org.apache.calcite.plan.RelOptCost;
import org.apache.calcite.plan.RelOptPlanner;
import org.apache.calcite.plan.RelTraitSet;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.RelCollation;
import org.apache.calcite.rel.core.Sort;
import org.apache.calcite.rel.metadata.RelMetadataQuery;
import org.apache.calcite.rex.RexLiteral;
import org.apache.calcite.rex.RexNode;

import java.lang.Math;

public class HuskyLogicalSort extends Sort implements HuskyLogicalRel {
	private int limitStart;

    public HuskyLogicalSort(RelOptCluster cluster, RelTraitSet traits, RelNode child, RelCollation collation, RexNode sortOffset, RexNode sortFetch) {
        super(cluster, traits, child, collation, sortOffset, sortFetch);
        if(offset != null) {
        	this.limitStart = RexLiteral.intValue(offset);
        } else {
        	this.limitStart = 0;
        }
    }

    @Override
    public Sort copy(RelTraitSet traitSet, RelNode newInput, RelCollation newCollation, RexNode offset, RexNode fetch) {
        return new HuskyLogicalSort(getCluster(), traitSet, newInput, newCollation, offset, fetch);
    }

    @Override
    public RelOptCost computeSelfCost(RelOptPlanner planner, RelMetadataQuery mq) {
    	// by default, assume cost is proportional to number of rows
    	double rowCnt = mq.getRowCount(this);
    	return planner.getCostFactory().makeCost(rowCnt, rowCnt, 0);
    }

    @Override
    public double estimateRowCount(RelMetadataQuery metadata) {
    	Double inputRowCnt = metadata.getRowCount(this.getInput());
    	if(inputRowCnt == null) {
    		return inputRowCnt.doubleValue();
    	} else {
    		double rowCount = Math.max((inputRowCnt.doubleValue() - limitStart), 1.0);
    		if(fetch != null) {
    			int limit = RexLiteral.intValue(fetch);
    			return Math.min(rowCount, limit);
    		} else {
    			return rowCount;
    		}
    	}
    }
}
