"use strict"
class UTWeb {
	//var serverType;
	
	constructor() {
		
	}
	
	/*
	Loads a given page within the server type folder
	if dirRoot is true the given page will be loaded from the directory root
	*/
	loadPage(page, dirRoot)
	{
		if (dirRoot === undefined) {
			dirRoot = false;
		}

		var fullPath;
		if (dirRoot)
			fullPath = page;
		else
			fullPath = serverType+"/"+page;
		
		$("#content").load(fullPath, function(responseTxt, statusTxt, xhr){
			//if (statusTxt == "error")
				//alert("Error: unable to load "+ fullPath);
		});
	}
}