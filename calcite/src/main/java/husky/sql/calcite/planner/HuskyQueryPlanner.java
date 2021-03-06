package husky.sql.calcite.planner;

import com.google.common.collect.ImmutableList;
import com.google.common.io.Resources;

import husky.sql.calcite.Exceptions.TableException;
import husky.sql.calcite.plan.nodes.HuskyConventions;
import husky.sql.calcite.plan.rules.HuskyRuleSets;
import org.apache.calcite.config.Lex;
import org.apache.calcite.jdbc.CalciteConnection;
import org.apache.calcite.model.ModelHandler;
import org.apache.calcite.plan.*;
import org.apache.calcite.plan.hep.HepMatchOrder;
import org.apache.calcite.plan.hep.HepPlanner;
import org.apache.calcite.plan.hep.HepProgramBuilder;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.RelRoot;
import org.apache.calcite.rel.RelVisitor;
import org.apache.calcite.rel.core.TableScan;
import org.apache.calcite.rel.type.RelDataTypeSystem;
import org.apache.calcite.schema.SchemaPlus;
import org.apache.calcite.sql.SqlNode;
import org.apache.calcite.sql.parser.SqlParseException;
import org.apache.calcite.sql.parser.SqlParser;
import org.apache.calcite.sql2rel.RelDecorrelator;
import org.apache.calcite.tools.*;

import java.io.IOException;
import java.nio.charset.Charset;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Iterator;
import java.util.Properties;

import static org.apache.calcite.plan.Contexts.EMPTY_CONTEXT;

public class HuskyQueryPlanner {

    private final FrameworkConfig config;
    
    public static void main(String[] args) throws IOException, SQLException, ValidationException, RelConversionException {
        if (args.length < 1) {
            System.out.println("usage: ./HuskyQueryPlanner \"<query string>\"");
        }
        Properties info = new Properties();
        info.setProperty("lex", "JAVA");
        CalciteConnection connection = DriverManager.getConnection("jdbc:calcite:", info)
                .unwrap(CalciteConnection.class);
        String schema = Resources.toString(SimpleQueryPlanner.class.getResource("/model.json"),
                Charset.defaultCharset());
        // ModelHandler reads the schema and load the schema to connection's root schema and sets the default schema
        new ModelHandler(connection, "inline:" + schema);

        // Create the query planner with the toy schema
        HuskyQueryPlanner queryPlanner = new HuskyQueryPlanner(connection.getRootSchema()
                .getSubSchema(connection.getSchema()));
        /* TODO - how to pass single quote mark from agrs*/
        RelRoot root = queryPlanner.getLogicalPlan("select L_RETURNFLAG, L_LINESTATUS, sum(L_QUANTITY) as SUM_QTY, sum(L_EXTENDEDPRICE) as SUM_BASE_PRICE, sum(L_EXTENDEDPRICE * (1 - L_DISCOUNT)) as SUM_DISC_PRICE, sum(L_EXTENDEDPRICE * (1 - L_DISCOUNT) * (1 + L_TAX)) as SUM_CHARGE, avg(L_QUANTITY) as AVG_QTY, avg(L_EXTENDEDPRICE) as AVG_PRICE, avg(L_DISCOUNT) as AVG_DISC, count(*) as COUNT_ORDER FROM Lineitem WHERE L_SHIPDATE <= DATE \'1998-12-01\' - INTERVAL \'90\' DAY GROUP BY L_RETURNFLAG, L_LINESTATUS ORDER BY L_RETURNFLAG, L_LINESTATUS");
        System.out.println("Initial logical plan: ");
        System.out.println(RelOptUtil.toString(root.rel));
        System.out.println(RelOptUtil.toString(queryPlanner.getPhysicalPlan(root)));
    }

    public HuskyQueryPlanner(SchemaPlus schema) {
        config = Frameworks.newConfigBuilder()
                // Lexical configuration defines how identifiers are quoted, whether they are converted to upper or lower
                // case when they are read, and whether identifiers are matched case-sensitively.
                .parserConfig(
                        SqlParser.configBuilder().setLex(Lex.MYSQL).build())
                .defaultSchema(schema) // Sets the schema to use by the planner
                .context(
                        EMPTY_CONTEXT) // Context can store data within the planner session for access by planner rules
                .ruleSets(
                        RuleSets.ofList()) // Rule sets to use in transformation phases
                .typeSystem(RelDataTypeSystem.DEFAULT)
                .build();
    }

    /**
     * run HEP planner
     */
    protected RelNode runHepPlanner(
            HepMatchOrder hepMatchOrder,
            RuleSet ruleSet,
            RelNode input,
            RelTraitSet targetTraits) {
        HepProgramBuilder builder = new HepProgramBuilder();
        builder.addMatchOrder(hepMatchOrder);

        Iterator<RelOptRule> it = ruleSet.iterator();
        while (it.hasNext()) {
            builder.addRuleInstance(it.next());
        }

        HepPlanner planner = new HepPlanner(builder.build(), config.getContext());
        planner.setRoot(input);
        if (input.getTraitSet() != targetTraits) {
            planner.changeTraits(input, targetTraits.simplify());
        }
        return planner.findBestExp();
    }

    protected RelNode runVolcanoPlanner(
            RelOptPlanner volcanoPlanner,
            RuleSet ruleSet,
            RelNode input,
            RelTraitSet targetTraits) {
        Program optProgram = Programs.ofRules(ruleSet);

        RelNode output = null;
        try {
            output = optProgram.run(volcanoPlanner, input, targetTraits,
                    ImmutableList.of(), ImmutableList.of());
        } catch (RelOptPlanner.CannotPlanException e) {
            throw e;
//            throw new TableException(
//                    "Cannot generate a valid execution plan for the given query: \n\n" +
//                            RelOptUtil.toString(input) + "\n" +
//                            "This exception indicates that the query uses an unsupported SQL feature.\n" +
//                            "Please check the documentation for the set of currently supported SQL features.");
        } catch (TableException t) {
            throw new TableException(
                    "Cannot generate a valid execution plan for the given query: \n\n" +
                            RelOptUtil.toString(input) + "\n" +
                            t.getMessage() + "\n" +
                            "Please check the documentation for the set of currently supported SQL features.");
        } catch (AssertionError a) {
            // keep original exception stack for caller
            throw a;
        }
        return output;
    }


    public RelRoot getLogicalPlan(String query) throws ValidationException, RelConversionException {
        SqlNode sqlNode;
        Planner planner = Frameworks.getPlanner(config);

        try {
            sqlNode = planner.parse(query);
        } catch (SqlParseException e) {
            throw new RuntimeException("Query parsing error.", e);
        }
        SqlNode validatedSqlNode = planner.validate(sqlNode);

        return planner.rel(validatedSqlNode);
    }

    public RelNode getPhysicalPlan(RelRoot root) {
        RelNode originalPlan = root.rel;
        RelOptPlanner volcanoPlanner = root.rel.getCluster().getPlanner();

        final RelVisitor visitor = new RelVisitor() {
            @Override
            public void visit(RelNode node, int ordinal, RelNode parent) {
                if (node instanceof TableScan) {
                    final RelOptCluster cluster = node.getCluster();
                    final RelOptTable.ToRelContext context =
                            RelOptUtil.getContext(cluster);
                    final RelNode r = node.getTable().toRel(context);
                    volcanoPlanner.registerClass(r);
                }
                super.visit(node, ordinal, parent);
            }
        };
        visitor.go(originalPlan);

        System.out.println("\nOriginal logical plan: ");
        System.out.println(RelOptUtil.toString(originalPlan) + "\n");

        // 0. convert sub-queries before query decorrelation
        RelNode convSubQueryPlan = runHepPlanner(
                HepMatchOrder.BOTTOM_UP, HuskyRuleSets.TABLE_SUBQUERY_RULES, originalPlan, originalPlan.getTraitSet());

        System.out.println("\nAfter step 0, convert sub-queries before query decorrelation: ");
        System.out.println(RelOptUtil.toString(convSubQueryPlan) + "\n");

        // 0. convert table references
        RelNode fullRelNode = runHepPlanner(
                HepMatchOrder.BOTTOM_UP,
                HuskyRuleSets.TABLE_REF_RULES,
                convSubQueryPlan,
                originalPlan.getTraitSet());

        System.out.println("\nAfter step 0, convert table references: ");
        System.out.println(RelOptUtil.toString(fullRelNode) + "\n");

        // 1. decorrelate
        RelNode decorPlan = RelDecorrelator.decorrelateQuery(fullRelNode);

        System.out.println("\nAfter step 1, decorrelate: ");
        System.out.println(RelOptUtil.toString(decorPlan) + "\n");

        // 2. normalize the logical plan
        RuleSet normRuleSet = HuskyRuleSets.NORM_RULES;
        RelNode normalizedPlan = decorPlan;
        if (normRuleSet.iterator().hasNext()) {
            runHepPlanner(HepMatchOrder.BOTTOM_UP, normRuleSet, decorPlan, decorPlan.getTraitSet());
        }

        System.out.println("\nAfter step 2, normalize the logical plan: ");
        System.out.println(RelOptUtil.toString(normalizedPlan) + "\n");

        // 3. optimize the logical Husky plan
        RuleSet logicalOptRuleSet = HuskyRuleSets.LOGICAL_OPT_RULES;
        RelTraitSet logicalOutputProps = originalPlan.getTraitSet().replace(HuskyConventions.LOGICAL).simplify();
        RelNode logicalPlan = normalizedPlan;
        if (logicalOptRuleSet.iterator().hasNext()) {
            logicalPlan = runVolcanoPlanner(volcanoPlanner, logicalOptRuleSet, normalizedPlan, logicalOutputProps);
        }

        System.out.println("\nAfter step 3, optimize the logical Husky plan: ");
        System.out.println(RelOptUtil.toString(logicalPlan) + "\n");

        return logicalPlan;
    }
}
