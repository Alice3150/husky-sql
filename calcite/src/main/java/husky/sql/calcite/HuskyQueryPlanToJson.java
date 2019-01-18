package husky.sql.calcite;

import com.google.common.io.Resources;

import husky.sql.calcite.planner.HuskyQueryPlanner;
import husky.sql.calcite.planner.SimpleQueryPlanner;
import husky.sql.calcite.plan.nodes.HuskyRelNode;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalRel;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalTableScan;
import husky.sql.calcite.plan.nodes.logical.HuskyLogicalCalc;
import husky.sql.calcite.table.SimpleTestTable;

import org.apache.calcite.jdbc.CalciteConnection;
import org.apache.calcite.model.ModelHandler;
import org.apache.calcite.plan.*;
import org.apache.calcite.rel.RelRoot;
import org.apache.calcite.tools.*;
import org.apache.calcite.rel.RelNode;
import org.apache.calcite.rel.type.RelDataTypeField;
import org.apache.calcite.rex.RexCall;
import org.apache.calcite.rex.RexLiteral;
import org.apache.calcite.rex.RexNode;
import org.apache.calcite.rex.RexInputRef;
import org.apache.calcite.rex.RexLocalRef;

import java.io.IOException;
import java.nio.charset.Charset;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;
import java.util.List;
import java.util.ArrayList;
import java.lang.AssertionError;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

import static org.apache.calcite.plan.Contexts.EMPTY_CONTEXT;

public class HuskyQueryPlanToJson {
    
    public static void main(String[] args) throws IOException, SQLException, ValidationException, RelConversionException {
        if (args.length < 1) {
            System.out.println("usage: ./HuskyQueryPlanToJson \"<query string>\"");
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
        RelRoot root = queryPlanner.getLogicalPlan(args[0]);
        RelNode logicalPlanRoot = queryPlanner.getPhysicalPlan(root);
        
        HuskyQueryPlanToJson convertJson = new HuskyQueryPlanToJson();
        RelOptTable logicalTable = logicalPlanRoot.getTable();
        List<String> names = logicalTable.getQualifiedName();
        SimpleTestTable simpletable = (SimpleTestTable) connection.getRootSchema().getSubSchema(names.get(0)).getTable(names.get(1));
        System.out.println(convertJson.getJson(logicalPlanRoot, simpletable)); 
    }

    private HuskyQueryPlanToJson() {}

    private JSONObject getJson(RelNode relRoot, SimpleTestTable simpletable) {
        JSONObject obj = new JSONObject();

        String relRootName = relRoot.getRelTypeName();
        obj.put("name", relRootName);

        // attributes by type
        if(relRootName.equals("HuskyLogicalTableScan") ){
            HuskyLogicalTableScan hlts = (HuskyLogicalTableScan)relRoot;
            List<String> allFieldNames = hlts.getTable().getRowType().getFieldNames();
            List<RelDataTypeField> allFieldTypes = hlts.getTable().getRowType().getFieldList();

            obj.put("table", hlts.getTable().getQualifiedName());
            obj.put("url", simpletable.getUrl());
            JSONArray fieldJsonArray = new JSONArray();
            for(int i = 0; i < allFieldNames.size(); i++) {
                JSONObject fieldJson = new JSONObject();
                fieldJson.put("index", Integer.toString(i));
                fieldJson.put("name", allFieldNames.get(i));
                fieldJson.put("datatype", allFieldTypes.get(i).getType().toString());
                fieldJsonArray.add(fieldJson);
            }
            obj.put("fields", fieldJsonArray);
            
            // projection
            JSONArray projectJsonArray = new JSONArray();

            for(int i = 0; i < hlts.fields.length; i++) {
                projectJsonArray.add(Integer.toString(hlts.fields[i]));
            }

            obj.put("projection", projectJsonArray);

            // condition
            RexCall condition = hlts.condition;
            JSONObject conditionJsonObject = getConditionJson(condition);
            obj.put("condition", conditionJsonObject);

            
        } else if(relRootName.equals("HuskyLogicalCalc")) {
            // projection
            // condition
        }

        JSONArray inputJsonArray = new JSONArray();
        List<RelNode> inputs = relRoot.getInputs();
        for(int i = 0; i < inputs.size(); i++) {
            JSONObject inputJson = getJson(inputs.get(i), simpletable);
            inputJson.put("input_index", Integer.toString(i));
            inputJsonArray.add(inputJson);
        }
        obj.put("inputs", inputJsonArray);
        return obj;
    }

    private JSONObject getConditionJson(RexNode rexNode) {
        JSONObject conditionJsonObject = new JSONObject();
        if(rexNode instanceof RexCall) {
            conditionJsonObject.put("type", "function");
            RexCall condition = (RexCall)rexNode;
            conditionJsonObject.put("name", condition.getOperator().getName());
            final List<RexNode> operands = condition.getOperands();

            JSONArray operandsJsonArray = new JSONArray();
            for(int i = 0; i < operands.size(); i++) {
                JSONObject opJson = getConditionJson(operands.get(i));
                opJson.put("op_index", Integer.toString(i + 1));
                operandsJsonArray.add(opJson);
            }
            conditionJsonObject.put("operands", operandsJsonArray);
        } else if(rexNode instanceof RexLocalRef) {
            conditionJsonObject.put("type", "field");
            RexLocalRef field = (RexLocalRef) rexNode;
            conditionJsonObject.put("index", field.getIndex());
            // conditionJsonObject.put("name", field.getName());
            conditionJsonObject.put("datatype", field.getType().toString());
        } else if(rexNode instanceof RexLiteral) {
            conditionJsonObject.put("type", "constant");
            RexLiteral constant = (RexLiteral) rexNode;
            // conditionJsonObject.put("datatype", constant.getTypeName().getName());
            conditionJsonObject.put("value", constant.getValue().toString());
        } else {
            System.out.println("RexNode class name: " + rexNode.getClass().getName());
            throw new AssertionError("unexpected");
        }

        return conditionJsonObject;
    }
}
