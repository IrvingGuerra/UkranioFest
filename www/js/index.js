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
				ip1: "192.168.0.7",
				ip2: "192.168.0.2",
				ip3: "192.168.0.3",
				ip4: "192.168.0.8"
			},
		    success: function(data){
		    	data = data.split('-');
		        showAlert('RES','verde','El porcentaje fue: '+data[0] + '% con tiempo de '+data[1]+'ms.');
		    }
		});
	}	
}