/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Core {

	#Context = null;

	constructor() {
	}

	navigateContents(urn) {
		document.location.href = urn;
	}

	objectStringify(object, depth = 0, max_depth = 2) {
		// change max_depth to see more levels, for a touch event, 2 is good

		if (depth > max_depth) {
			return "Object";
		}


		const obj = {};


		for (let key in object) {
			let value = object[key];


			if (value instanceof Node) {
				// specify which properties you want to see from the node
				value = { id: value.id };
			}
			else if (value instanceof Window) {
				value = "Window";
			}
			else if (value instanceof Object) {
				value = this.objectStringify(value, depth + 1, max_depth);
			}

			obj[key] = value;
		}


		return depth ? obj : JSON.stringify(obj);
	}

	contentsPostMessage(o) {
		//console.log("contentsPostMessage(): "+ o.target, o);

		let v = this.objectStringify(o);


		if (v != undefined) {
			/*
			let s;
			
			
			s = v.replaceAll("\\\"", "\"");
			alert(s);
			console.log("contentsPostMessage(): objectStringify() = " + s);
			
			
			window.chrome.webview.postMessage(s);
			*/
			window.chrome.webview.postMessage(v);
		}
		else {
			console.log("contentsPostMessage(): objectStringify() = undefined");
		}
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
var _Core = null;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
function coreInitialize() {
	_Core = new Core();
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
//window.onload = function () {
//	coreInitialize();
//}




