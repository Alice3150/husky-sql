package husky.sql.calcite.plan.nodes.logical.converter;

import husky.sql.calcite.plan.nodes.HuskyConventions;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalSort;
import org.apache.calcite.plan.Convention;
import org.apache.calcite.plan.RelOptRule;
import org.apache.calcite.plan.RelTraitSet;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.convert.ConverterRule;
import org.apache.calcite.rel.logical.LogicalSort;

public class HuskyLogicalSortConverter extends ConverterRule {

    public static HuskyLogicalSortConverter INSTANCE = new HuskyLogicalSortConverter();

    public HuskyLogicalSortConverter() {
        super(LogicalSort.class, Convention.NONE, HuskyConventions.LOGICAL, "HuskyLogicalSortConverter");
    }

    @Override
    public RelNode convert(RelNode rel) {
        LogicalSort sort = (LogicalSort)rel;
        RelTraitSet traitSet = rel.getTraitSet().replace(HuskyConventions.LOGICAL);
        RelNode newInput = RelOptRule.convert(sort.getInput(), HuskyConventions.LOGICAL);

        return new HuskyLogicalSort(rel.getCluster(), traitSet, newInput, sort.getCollation(), sort.offset, sort.fetch);
    }
}