var SWFUpload;
if (typeof(SWFUpload) === "function") {
	SWFUpload.gracefulDegradation = {};
	SWFUpload.prototype.initSettings = (function (oldInitSettings) {
		return function () {
			if (typeof(oldInitSettings) === "function") {
				oldInitSettings.call(this);
			}
			
			this.ensureDefault = function (settingName, defaultValue) {
				this.settings[settingName] = (this.settings[settingName] == undefined) ? defaultValue : this.settings[settingName];
			};
			
			this.ensureDefault("swfupload_element_id", "swfupload_container");
			this.ensureDefault("degraded_element_id", "degraded_container");
			this.settings.user_swfupload_loaded_handler = this.settings.swfupload_loaded_handler;

			this.settings.swfupload_loaded_handler = SWFUpload.gracefulDegradation.swfUploadLoadedHandler;
			
			delete this.ensureDefault;
		};
	})(SWFUpload.prototype.initSettings);

	SWFUpload.gracefulDegradation.swfUploadLoadedHandler = function () {
		var swfuploadContainerID, swfuploadContainer, degradedContainerID, degradedContainer;

		swfuploadContainerID = this.settings.swfupload_element_id;
		degradedContainerID = this.settings.degraded_element_id;
		
		// Show the UI container
		swfuploadContainer = document.getElementById(swfuploadContainerID);
		if (swfuploadContainer != undefined) {
			swfuploadContainer.style.display = "block";

			// Now take care of hiding the degraded UI
			degradedContainer = document.getElementById(degradedContainerID);
			if (degradedContainer != undefined) {
				degradedContainer.style.display = "none";
			}
		}
		
		if (typeof(this.settings.user_swfupload_loaded_handler) === "function") {
			this.settings.user_swfupload_loaded_handler.apply(this);
		}
	};

}
