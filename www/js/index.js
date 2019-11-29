var fileName = "";

function fileSelect(event){
    fileName = event.target.files[0].name;
    console.log(fileName);
}

function start() {
	if (fileName!="") {
		$.ajax({
			type: 'POST',
		    url: '/sendFile',
		    data: {
				fileName: fileName,
				ip1: "192.168.0.2",
				ip2: "192.168.0.6",
				ip3: "192.168.0.3"
			},
		    success: function(data){
		        showAlert('RES','verde','El porcentaje fue: '+data);
		    }
		});
	}	
}