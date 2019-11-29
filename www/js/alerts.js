function openModal(title,description,btn1,btn2,fnbtn1,fnbtn2) {
	$('body').append('<div class="modal"><div class="submodal"></div></div>');
	$('.submodal').append('<span class="closebtnModal" onclick="closeModal();">&times;</span>');
	$('.submodal').append('<h1>'+title+'</h1>');
	$('.submodal').append('<h2>'+description+'</h2>');
	if (btn1 != null) {
		$('.submodal').append('<button class="btnFormal" onclick="'+fnbtn1+'">'+btn1+'</button>');
	}
	if (btn2 != null) {
		$('.submodal').append('<button class="btnFormal rojo" onclick="'+fnbtn2+'">'+btn2+'</button>');
	}
	$('.modal').fadeIn();
}

function closeModal() {
	$('#beep')[0].play();
	$('.modal').fadeOut();
	setTimeout(function () {$('.modal').remove();}, 500);
}
//Funciones para alertas pequeñas
function showAlert(id,color,text) {
	desplazaA('body');
	$('#alertas').html(""); //Limpiamos el div especializado para alertas
	$('#alertas').append('<div class="alert '+color+'"></div>');
	$('.alert').append('<span class="closebtnAlert" onclick="closeAlert();">&times;</span>');
	$('.alert').append('<label>'+text+'</label>');
	$('.alert').addClass(color);
	$('.alert').fadeIn();
	//setTimeout(function () {$('.alert').fadeOut();}, 2000);
}
function closeAlert(){
	$('.alert').fadeOut();
}

String.prototype.replaceAll = function(search, replacement) {
    var target = this;
    return target.replace(new RegExp(search, 'g'), replacement);
};

function desplazaA(url) {
    var destino = $(url);
    if (destino.length == 0) {
      destino = $('a[name="' + this.hash.substr(1) + '"]');
    }
    if (destino.length == 0) {
      destino = $('html');
    }
    $('html, body').animate({ scrollTop: destino.offset().top }, 800);
}

function formatMoney(amount, decimalCount = 0, decimal = ".", thousands = ",") {
  try {
    decimalCount = Math.abs(decimalCount);
    decimalCount = isNaN(decimalCount) ? 2 : decimalCount;

    const negativeSign = amount < 0 ? "-" : "";

    let i = parseInt(amount = Math.abs(Number(amount) || 0).toFixed(decimalCount)).toString();
    let j = (i.length > 3) ? i.length % 3 : 0;

    return negativeSign + (j ? i.substr(0, j) + thousands : '') + i.substr(j).replace(/(\d{3})(?=\d)/g, "$1" + thousands) + (decimalCount ? decimal + Math.abs(amount - i).toFixed(decimalCount).slice(2) : "");
  } catch (e) {
    console.log(e)
  }
};