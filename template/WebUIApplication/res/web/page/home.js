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

	navigatePage(page){
		let jsonMessage =
		{
			Command: "페이지이동",
			Page: page
		};

		_core.contentsPostMessage(jsonMessage);
	}
	
	setupNavigateButton(targetName, page) {
		let targetElement;

		targetElement = document.getElementById(name);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					this.navigatePage(page);
				}
			);
		}
	}
	
	setup() {
		this.setupNavigateButton("", "");
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


