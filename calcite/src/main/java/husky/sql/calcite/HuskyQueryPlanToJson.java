package husky.sql.calcite;

import com.google.common.io.Resources;

import husky.sql.calcite.planner.HuskyQueryPlanner;
import husky.sql.calcite.planner.SimpleQueryPlanner;
import husky.sql.calcite.plan.nodes.HuskyRelNode;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalRel;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalTableScan;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalCalc;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalAggregate;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalSort;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalJoin;
import husky.sql.calcite.table.SimpleTestTable;

import org.apache.calcite.jdbc.CalciteConnection;
import org.apache.calcite.model.ModelHandler;
import org.apache.calcite.plan.*;
import org.apache.calcite.rel.RelRoot;
import org.apache.calcite.tools.*;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.core.AggregateCall;
import org.apache.calcite.rel.type.RelDataTypeField;
import org.apache.calcite.rel.RelFieldCollation;
import org.apache.calcite.rel.externalize.RelJsonWriter;
import org.apache.calcite.rex.RexCall;
import org.apache.calcite.rex.RexLiteral;
import org.apache.calcite.rex.RexNode;
import org.apache.calcite.rex.RexInputRef;
import org.apache.calcite.rex.RexLocalRef;
import org.apache.calcite.rex.RexProgram;
import org.apache.calcite.linq4j.Ord;
import org.apache.calcite.util.Pair;

import java.io.IOException;
import java.nio.charset.Charset;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;
import java.util.List;
import java.util.ArrayList;
import java.lang.AssertionError;
import java.io.FileWriter;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import static org.apache.calcite.plan.Contexts.EMPTY_CONTEXT;

public class HuskyQueryPlanToJson {
    
    public static void main(String[] args) throws IOException, SQLException, ValidationException, RelConversionException {
        if (args.length < 2) {
            System.out.println("usage: ./HuskyQueryPlanToJson \"<query string>\" \"<file path>\"");
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
        RelRoot logicalPlan = queryPlanner.getLogicalPlan("select L_ORDERKEY, sum(L_EXTENDEDPRICE*(1-L_DISCOUNT)) as REVENUE, O_ORDERDATE, O_SHIPPRIORITY from Customer, Orders, Lineitem where C_MKTSEGMENT = \'BUILDING\' and C_CUSYKEY = O_CUSTKEY and L_ORDERKEY = O_ORDERKEY and O_ORDERDATE < date \'1995-03-15\' and l_shipdate > date \'1995-03-15\' group by L_ORDERKEY, O_ORDERDATE, O_SHIPPRIORITY order by REVENUE desc, O_ORDERDATE");
        RelNode physicalPlan = queryPlanner.getPhysicalPlan(logicalPlan);
        
        RelNode physicalPlanTableScan = queryPlanner.getPhysicalPlan(logicalPlan);
        HuskyQueryPlanToJson convertJson = new HuskyQueryPlanToJson();
        RelOptTable logicalTable = null;
        if (!(physicalPlanTableScan instanceof HuskyLogicalTableScan)) {
            RelNode itr_node = physicalPlanTableScan;
            while(itr_node.getInputs().size() > 0) {
                    itr_node = itr_node.getInput(0);
                    if(itr_node instanceof HuskyLogicalTableScan)
                        logicalTable = itr_node.getTable();
            }
            if (logicalTable == null) {
                System.out.println("Can not find TableScan.\n Exit!");
                System.exit(1);
            }

        }
        
        JSONObject jsonPlan = convertJson.getJson(physicalPlan, connection);

        String outputJsonPath = args[1];
        try (FileWriter file = new FileWriter(outputJsonPath)) {
            file.write(jsonPlan.toString());
            file.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println(jsonPlan);
    }

    private HuskyQueryPlanToJson() {}

    private JSONObject getJson(RelNode relRoot, CalciteConnection connection) {
        JSONObject obj = new JSONObject();

        String relRootName = relRoot.getRelTypeName();
        obj.put("name", relRootName);

        // attributes by type
        if(relRootName.equals("HuskyLogicalTableScan") ){
            HuskyLogicalTableScan hlts = (HuskyLogicalTableScan)relRoot;
            List<String> allFieldNames = hlts.getTable().getRowType().getFieldNames();
            List<RelDataTypeField> allFieldTypes = hlts.getTable().getRowType().getFieldList();

            List<String> names = hlts.getTable().getQualifiedName();
            obj.put("table", names);

            SimpleTestTable simpletable = (SimpleTestTable) connection.getRootSchema().getSubSchema(names.get(0)).getTable(names.get(1));
            obj.put("url", simpletable.getUrl());
            JSONArray fieldJsonArray = new JSONArray();
            for(int i = 0; i < allFieldNames.size(); i++) {
                JSONObject fieldJson = new JSONObject();
                fieldJson.put("index", i);
                fieldJson.put("name", allFieldNames.get(i));
                fieldJson.put("datatype", allFieldTypes.get(i).getType().getSqlTypeName().getName());
                fieldJsonArray.add(fieldJson);
            }
            obj.put("fields", fieldJsonArray);
            
            // projection
            JSONArray projectJsonArray = new JSONArray();
            for(int i = 0; i < hlts.fields.length; i++) {
                projectJsonArray.add(hlts.fields[i]);
            }
            obj.put("projection", projectJsonArray);

            // condition
            RexCall condition = hlts.condition;
            if(condition != null) {
                JSONObject conditionJsonObject = getRexNode(condition, null);
                obj.put("condition", conditionJsonObject);
            }

            
        } else if(relRootName.equals("HuskyLogicalCalc")) {
            HuskyLogicalCalc hlc = (HuskyLogicalCalc)relRoot;
            RexProgram program = hlc.getProgram();
            List<RexNode> exps = program.getExprList();
            
            List<RexLocalRef> projectList = program.getProjectList();
            JSONArray projectJsonArray = new JSONArray();
            for(RexLocalRef proj : projectList) {
                RexNode proj_rex = exps.get(proj.getIndex());
                JSONObject projJsonObject = getRexNode(proj_rex, program);
                projectJsonArray.add(projJsonObject);
                
            }
            obj.put("projection", projectJsonArray);

            // condition
            RexLocalRef cond_ref = program.getCondition();
            if(cond_ref != null) {
                JSONObject condJsonObject = getRexNode(cond_ref, program);
                obj.put("condition", condJsonObject);
            }

        } else if(relRootName.equals("HuskyLogicalAggregate")) {
            HuskyLogicalAggregate hla = (HuskyLogicalAggregate)relRoot;

            // group set
            int[] groupSet = hla.getGroupSet().toArray();
            JSONArray groupSetJsonArray = new JSONArray();
            for(int i = 0; i < groupSet.length; i++) {
                groupSetJsonArray.add(groupSet[i]);
            }
            obj.put("group", groupSetJsonArray);

            // aggregate calls
            List<AggregateCall> aggCalls = hla.getAggCallList();
            JSONArray aggCallsJsonArray = new JSONArray();
            for(AggregateCall aggCall : aggCalls) {
                JSONObject aggCallJsonObject = getAggregateCall(aggCall);
                aggCallsJsonArray.add(aggCallJsonObject);
            }
            obj.put("aggregateCalls", aggCallsJsonArray);

         } else if(relRootName.equals("HuskyLogicalSort")) {
            HuskyLogicalSort hls  = (HuskyLogicalSort)relRoot;
            List<RexNode> fieldExps = hls.getChildExps();
            List<RelFieldCollation>  fieldCollations = hls.getCollation().getFieldCollations();

            JSONArray sortJsonArray = new JSONArray();
            for (RexNode field : fieldExps) {
                JSONObject sortJsonObject = getRexNode(field, null);
                sortJsonArray.add(sortJsonObject);
            }
            obj.put("sort", sortJsonArray);

            JSONArray dirJsonArray = new JSONArray();
            for (RelFieldCollation fieldCollation : fieldCollations) {
                dirJsonArray.add(fieldCollation.shortString());
            }
            obj.put("dir", dirJsonArray);
         } else if(relRootName.equals("HuskyLogicalJoin")) {
            HuskyLogicalJoin hlj = (HuskyLogicalJoin)relRoot;

            RexNode condition = hlj.getCondition();
            if(condition != null) {
                JSONObject conditionJsonObject = getRexNode(condition, null);
                obj.put("condition", conditionJsonObject);
            }

            String joinType = hlj.getJoinType().toString();
            obj.put("type", joinType);
         }

        JSONArray inputJsonArray = new JSONArray();
        List<RelNode> inputs = relRoot.getInputs();
        for(int i = 0; i < inputs.size(); i++) {
            JSONObject inputJson = getJson(inputs.get(i), connection);
            // inputJson.put("input_index", Integer.toString(i));
            inputJsonArray.add(inputJson);
        }
        obj.put("inputs", inputJsonArray);
        return obj;
    }

    private JSONObject getRexNode(RexNode rexNode, RexProgram program) {
        JSONObject conditionJsonObject = new JSONObject();
        if(rexNode instanceof RexCall) {
            conditionJsonObject.put("type", "function");
            RexCall condition = (RexCall)rexNode;
            conditionJsonObject.put("name", condition.getOperator().getName());
            final List<RexNode> operands = condition.getOperands();

            JSONArray operandsJsonArray = new JSONArray();
            for(int i = 0; i < operands.size(); i++) {
                JSONObject opJson = getRexNode(operands.get(i), program);
                operandsJsonArray.add(opJson);
            }
            conditionJsonObject.put("operands", operandsJsonArray);
        } else if(rexNode instanceof RexLocalRef) {
            /* expand local ref */
            RexLocalRef localRef = (RexLocalRef) rexNode;
            if(program != null) {
                return getRexNode(program.expandLocalRef(localRef), program);
            } else {
                conditionJsonObject.put("index", localRef.getIndex());
                conditionJsonObject.put("type", "inputRef"); // For condition in HuskyLogicalTableScan
                conditionJsonObject.put("datatype", localRef.getType().getSqlTypeName().getName());
            }
        } else if(rexNode instanceof RexLiteral) {
            conditionJsonObject.put("type", "constant");
            RexLiteral constant = (RexLiteral) rexNode;
            conditionJsonObject.put("datatype", constant.getTypeName().getName());
            conditionJsonObject.put("value", constant.toString());
        } else if(rexNode instanceof RexInputRef) {
            conditionJsonObject.put("type", "inputRef");
            RexInputRef input = (RexInputRef)rexNode;
            conditionJsonObject.put("index", input.getIndex());
            conditionJsonObject.put("datatype", input.getType().getSqlTypeName().getName());
        } else {
            System.out.println("RexNode class name: " + rexNode.getClass().getName());
            throw new AssertionError("unexpected");
        }

        return conditionJsonObject;
    }

    private JSONObject getAggregateCall(AggregateCall aggCall) {
        JSONObject aggCallJsonObject = new JSONObject();
        aggCallJsonObject.put("name", aggCall.getName());
        aggCallJsonObject.put("datatype", aggCall.getType().getSqlTypeName().getName());
        aggCallJsonObject.put("function", aggCall.getAggregation().getName());
        JSONArray args = new JSONArray();
        for(Integer i : aggCall.getArgList())
            args.add(i);
        aggCallJsonObject.put("args", args);
        return aggCallJsonObject;
    }
}
