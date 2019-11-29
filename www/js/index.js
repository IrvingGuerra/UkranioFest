var fileName = "";

function fileSelect(event){
    fileName = event.target.files[0].name;
    console.log(fileName);
}

function start() {
	$('#btnAnalizar').fadeOut();
	$('#loaderImg').fadeIn();


	var form = $('#fileUploadForm')[0];

    var data = new FormData(form);
    data.append("ips", "192.168.0.6 192.168.0.4 192.168.0.2 192.168.0.7");

	if (fileName!="") {
		$.ajax({
			type: 'POST',
		    url: '/sendFile',
		    cache: false,
		    enctype: 'multipart/form-data',
		    contentType: false,
		    processData: false,
		    data: data,
		    success: function(data){
		    	data = data.split('-');
		        showAlert('RES','verde','El porcentaje fue: '+data[0] + '% con tiempo de '+data[1]+'ms.');
		    	$('#btnAnalizar').fadeIn();
		    	$('#loaderImg').fadeOut();
		    }
		});
	}	
}