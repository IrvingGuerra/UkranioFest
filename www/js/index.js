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
				fileName: fileName
				ip1: "192.168.0.1",
				ip2: "192.168.0.1",
				ip3: "192.168.0.1"
			},
		    success: function(data){
		        alert(data);
		    }
		});
	}	
}



/*

$.ajax({
	type: "POST",
	url: "/game",
	data: {
		boardId: idTablero,
		playerName: "BOT4",
		playerIsBot: true,
		playerColor: "#138F00"
	},
	success: function (data) {
		if (data == "SUCCESS") {
			$('#beep')[0].play();
			setTimeout(function(){ 
				setCookie('idTablero', idTablero, 7);
				setCookie('username', $('#username').val(), 7);
				window.location = 'monopoly.html';
			}, 300);
		}
	}
});

*/