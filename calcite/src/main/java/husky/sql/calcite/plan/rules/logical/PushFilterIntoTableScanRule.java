package husky.sql.calcite.plan.rules.logical;

import husky.sql.calcite.plan.util.*;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalTableScan;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalCalc;
import org.apache.calcite.plan.RelOptRule;
import org.apache.calcite.plan.RelOptRuleCall;
import org.apache.calcite.rel.core.RelFactories;
import org.apache.calcite.rel.logical.LogicalProject;
import org.apache.calcite.rex.*;
import org.apache.calcite.tools.RelBuilderFactory;
import org.apache.calcite.tools.RelBuilder;

import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.stream.Collectors;
import java.util.Optional;

public class PushFilterIntoTableScanRule extends RelOptRule {
    public static final PushFilterIntoTableScanRule INSTANCE =
      new PushFilterIntoTableScanRule();

    public PushFilterIntoTableScanRule() {
        super(operand(HuskyLogicalCalc.class, 
            operand(HuskyLogicalTableScan.class, none())), 
            "PushFilterIntoTableScanRule");
    }

    @Override
    public boolean matches(RelOptRuleCall call) {
        HuskyLogicalCalc calc = call.rel(0);
        HuskyLogicalTableScan scan = call.rel(1);

        // only continue if condition in calc is not null and we haven't pushed down a Filter yet.
        return calc.getProgram().getCondition() != null && !scan.isFilterPushDown();
    }

    @Override
    public void onMatch(RelOptRuleCall call) { 
        HuskyLogicalCalc calc = call.rel(0);
        HuskyLogicalTableScan scan = call.rel(1);

        if(scan.isFilterPushDown()) {
            return;
        }

        RexProgram program = calc.getProgram();
        // RexNode condition = program.expandLocalRef(program.getCondition());
        List<RexNode> remainingConditions = RexProgramExtractor
                                            .extractConjunctiveConditions(program, call.builder().getRexBuilder());
        if(remainingConditions.isEmpty()) {
            // no condition can be translated to expression
            return;
        }

        // check whether framework still need to do a filter
        RelBuilder relBuilder = call.builder();
        RexNode remainingCondition;
        if(!remainingConditions.isEmpty()) {
            // relBuilder.push(scan);
            Optional<RexNode> resultCondition = remainingConditions.stream()
                                                    .reduce((l, r) -> relBuilder.and(l, r));
            if(resultCondition.isPresent()) {
                remainingCondition = resultCondition.get();
            } else {
                remainingCondition = null;
            }
        } else {  
            remainingCondition = null;
        }

        // System.out.println("remainingCondition: " + remainingCondition.toString());
        // System.out.println("Input fields of Calc: " + Arrays.toString(program.getInputRowType().getFieldNames().toArray()));        // check whether we still need a RexProgram. An RexProgram is needed when either
        
        // projection or filter exists.
        HuskyLogicalTableScan newScan = scan.copy(scan.getTraitSet(), scan.fields);
        if(scan.fields.length == 0) {
        	// no projection in table scan; directly copy condition
            // System.out.println("--- Used Fields with no projections in scan: " + Arrays.toString(RexProgramExtractor.extractRefInputFields(program)));
            // newScan.applyPredicate(remainingCondition, RexProgramExtractor.extractRefInputFields(program));
            newScan.applyPredicateByCopy(remainingCondition);
        } else {
        	// projection has been pushed down; convert back to original index
            // System.out.println("--- Used Fields with projections " + Arrays.toString(scan.fields) + " in scan: " + Arrays.toString(RexProgramExtractor.extractRefInputFields(program)));
            newScan.applyPredicate(remainingCondition, scan.fields);
        }
    
        RexProgram newCalProgram;
        // if(remainingCondition != null || !program.projectsOnlyIdentity()) {
        if(!program.projectsOnlyIdentity()) {
            List<RexNode> expandedProjectList = program.getProjectList().stream()
                                                .map(ref -> program.expandLocalRef(ref))
                                                .collect(Collectors.toList());
            newCalProgram = RexProgram.create(
                                        program.getInputRowType(), 
                                        expandedProjectList,
                                        // remainingCondition,
                                        null,
                                        program.getOutputRowType(),
                                        relBuilder.getRexBuilder());
        } else {
            newCalProgram = null;
        }

        if(newCalProgram != null) {
            // System.out.println("--- push filter: transform to Calc");
            call.transformTo(calc.copy(calc.getTraitSet(), newScan, newCalProgram));
        } else {
            // System.out.println("--- push filter: transform to TableScan");
            call.transformTo(newScan);
        }
    }

    private int[] getProjectFields(List<RexNode> exps) {
        final int[] fields = new int[exps.size()];
        for(int i = 0; i < exps.size(); i++) {
            final RexNode exp = exps.get(i);
            if(exp instanceof RexInputRef) {
                fields[i] = ((RexInputRef) exp).getIndex();
            } else {
                return null; // not a simple projection
            }
        }
        return fields;
    }
}