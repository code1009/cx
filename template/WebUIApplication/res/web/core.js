/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
function escapeHtml(str) {
	const map = {
		'&': '&amp;',
		'<': '&lt;',
		'>': '&gt;',
		'"': '&quot;',
		"'": '&#39;'
	};
	return str.replace(/[&<>"']/g, function (m) { return map[m]; });
}

function unescapeHtml(str) {
	const map = {
		'&amp;': '&',
		'&lt;': '<',
		'&gt;': '>',
		'&quot;': '"',
		'&#39;': "'"
	};
	return str.replace(/&(amp|lt|gt|quot|#39);/g, function (m) { return map[m]; });
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class BasePage {

	//-----------------------------------------------------------------------
	_CommandHandlerMap = null;

	//-----------------------------------------------------------------------
	constructor() {
	}

	//-----------------------------------------------------------------------
	navigateContents(urn) {
		document.location.href = urn;
	}

	//-----------------------------------------------------------------------
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

	postJsonMessage(o) {
		//console.log("postJsonMessage(): "+ o.target, o);

		let v = this.objectStringify(o);
		if (v != undefined) {
			/*
			let s;
			
			
			s = v.replaceAll("\\\"", "\"");
			alert(s);
			console.log("postJsonMessage(): objectStringify() = " + s);
			
			
			window.chrome.webview.postMessage(s);
			*/
			window.chrome.webview.postMessage(v);
		}
		else {
			console.log("postJsonMessage(): objectStringify() = undefined");
		}
	}

	//-----------------------------------------------------------------------
	setupClickEventListener(targetName, methodName, methodParam) {
		const targetElement = document.getElementById(targetName);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					if (typeof this[methodName] === 'function') {
						this[methodName](methodParam);
					}
				}
			);
		}
	}

	postNavigatePage(page){
		let jsonMessage =
		{
			Command: "페이지이동",
			TargetPage: page
		};
		this.postJsonMessage(jsonMessage);
	}

	postMessageString(messageString){
		let jsonMessage =
		{
			Command: "메시지",
			MessageString: messageString
		};
		this.postJsonMessage(jsonMessage);
	}

	//-----------------------------------------------------------------------
	setBkColor(targetName, color) {
		const targetElement = document.getElementById(targetName);
		if (targetElement) {
			switch(color) {
			case 0:
				targetElement.style.backgroundColor = "#F0F0F0";
				break;
			case 1:
				targetElement.style.backgroundColor = "#FFFF00";
				break;
			case 2:
				targetElement.style.backgroundColor = "#FF0000";
				break;
			}
		}
	}

	//-----------------------------------------------------------------------
	setupWebMessageHandler() {
		window.chrome.webview.addEventListener(
			"message",
			arg => {
				this.onWebMessage(arg);
			}
		);
	}

	onWebMessage(arg) {
		if ("Command" in arg.data)
		{
			this.onCommand(arg.data);
		}
	}

	onCommand(argData) {
		const commandHandler = this._CommandHandlerMap.get(argData.Command);
		if (commandHandler) {
			commandHandler(argData);
		} 
		else {
			console.warn(`Unknown command: ${argData.Command}`);
		}
	}
}


