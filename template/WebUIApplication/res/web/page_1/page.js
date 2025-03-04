/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";




/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Page {

	#Context = null;

	constructor() {
		this.#Context = null;
		this.setup();
	}

	setup() {
		this.setupWebMessageHandler();
		this.setupPostNavigateButton("페이지이동-페이지1", "집");
		this.setupPostMessageButton("메시지1", "JS문자열");
	}

	//-----------------------------------------------------------------------
	setupPostNavigateButton(targetName, page) {
		let targetElement;

		targetElement = document.getElementById(targetName);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					this.postNavigatePage(page);
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
		_Core.contentsPostMessage(jsonMessage);
	}

	//-----------------------------------------------------------------------
	setupPostMessageButton(targetName, messageString) {
		let targetElement;

		targetElement = document.getElementById(targetName);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					this.postMessageString(messageString);
				}
			);
		}
	}

	postMessageString(messageString){
		let jsonMessage =
		{
			Command: "메시지",
			MessageString: messageString
		};
		_Core.contentsPostMessage(jsonMessage);
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
		if ("메시지"==argData.Command) {
			this.onMessageStringCommand(argData);
		}
		else {
			
		}
	}

	onMessageStringCommand(argData) {
		alert(argData.MessageString);
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
var _Page = null;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
window.onload = function () {
	coreInitialize();
	_Page = new Page();
}


