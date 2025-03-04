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
		this.setupNavigateButton("페이지이동-페이지1", "페이지1");
		this.setupNavigateButton("페이지이동-페이지2", "페이지2");
		this.setupNavigateButton("페이지이동-페이지3", "페이지3");
	}

	setupNavigateButton(targetName, page) {
		let targetElement;

		targetElement = document.getElementById(targetName);
		if (targetElement != null) {
			targetElement.addEventListener(
				"click",
				(e) => {
					this.navigatePage(page);
				}
			);
		}
	}

	navigatePage(page){
		let jsonMessage =
		{
			Command: "페이지이동",
			TargetPage: page
		};

		_Core.contentsPostMessage(jsonMessage);
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


