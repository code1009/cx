﻿/////////////////////////////////////////////////////////////////////////////
//===========================================================================
"use strict";





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class Page extends BasePage {

	//-----------------------------------------------------------------------
	constructor() {
		super();
		this.setup();
	}

	//-----------------------------------------------------------------------
	setup() {
		this.setupClickEventListener("페이지1", "postNavigatePage", "페이지1");
		this.setupClickEventListener("페이지2", "postNavigatePage", "집");
		this.setupClickEventListener("페이지3", "postNavigatePage", "페이지3");
	}
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
var _Page = null;





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
window.onload = function () {
	_Page = new Page();
}


