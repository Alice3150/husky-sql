package husky.sql.calcite.plan.util;

import org.apache.calcite.rex.*;
import org.apache.calcite.rel.type.RelDataType;

import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.stream.Collectors;

public class InputRefRegister {
	// copy from calcite

	// private class RegisterInputShuttle extends RegisterShuttle {
	//     private final boolean valid;

	//     protected RegisterInputShuttle(boolean valid) {
	//       this.valid = valid;
	//     }

	//     public RexNode visitInputRef(RexInputRef input) {
	//       final int index = input.getIndex();
	//       if (valid) {
	//         // The expression should already be valid. Check that its
	//         // index is within bounds.
	//         if ((index < 0) || (index >= inputRowType.getFieldCount())) {
	//           assert false
	//               : "RexInputRef index " + index + " out of range 0.."
	//               + (inputRowType.getFieldCount() - 1);
	//         }

	//         // Check that the type is consistent with the referenced
	//         // field. If it is an object type, the rules are different, so
	//         // skip the check.
	//         assert input.getType().isStruct()
	//             || RelOptUtil.eq("type1", input.getType(),
	//                 "type2", inputRowType.getFieldList().get(index).getType(),
	//                 Litmus.THROW);
	//       }

	//       // Return a reference to the N'th expression, which should be
	//       // equivalent.
	//       final RexLocalRef ref = localRefList.get(index);
	//       return ref;
	//     }

	//     public RexNode visitLocalRef(RexLocalRef local) {
	// 		if (valid) {
	// 		    // The expression should already be valid.
	// 		    final int index = local.getIndex();
	// 		    assert index >= 0 : index;
	// 		    assert index < exprList.size()
	// 		        : "index=" + index + ", exprList=" + exprList;
	// 		    assert RelOptUtil.eq(
	// 		        "expr type",
	// 		        exprList.get(index).getType(),
	// 		        "ref type",
	// 		        local.getType(),
	// 		        Litmus.THROW);
	// 		}

	// 		  	// Resolve the expression to an input.
	// 		  	while (true) {
	// 				final int index = local.getIndex();
	// 				final RexNode expr = exprList.get(index);
	// 				if (expr instanceof RexLocalRef) {
	// 				  	local = (RexLocalRef) expr;
	// 				  	if (local.index >= index) {
	// 				    	throw new AssertionError(
	// 				       		"expr " + local + " references later expr " + local.index);
	// 				  }
	// 				} else {
	// 				  	// Add expression to the list, just so that subsequent
	// 				  	// expressions don't get screwed up. This expression is
	// 				  	// unused, so will be eliminated soon.
	// 				  	return registerInternal(local, false);
	// 				}
	// 		  }
	//     }
 //  	}

 //  	public RexLocalRef registerInput(RexNode expr) {
	//     final RexShuttle shuttle = new RegisterInputShuttle(true);
	//     final RexNode ref = expr.accept(shuttle);
	//     return (RexLocalRef) ref;
 //  	}
}