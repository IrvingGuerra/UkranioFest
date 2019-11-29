function configureProfile() {
	$('#beep')[0].play();
	setTimeout(function(){ 
		window.location = 'configure.html';
	}, 300);
}
function showRules() {
	$('#beep')[0].play();

	var rules = "<div style='font-size:15px'>BANCO: </br></br>"+
	"1. El Banco cobra todos los castigos que aparezcan en las tarjetas rojas.</br>"+
	"2. El Banco da el dinero de los premios que aparezcan en las tarjetas azules.</br>"+
	"3. El banco puede prestar dinero.</br>"+
	"4. El juego termina cuando el Banco se queda sin dinero, por lo tanto se deduce que el Banco tiene dinero limitado establecido por los usuarios.</br></br>"+
	"JUGADORES:</br></br>"+
	"1. El turno del jugador es determinado por el momento en el que ingresa a una partida.</br>"+
	"2. Cada jugador tiene la oportunidad de girar el dado cuando sea su turno, a menos que haya recibido un castigo que lo condiciones a no hacerlo.</br>"+
	"3. Dos o mas fichas pueden estar en la misma casilla al mmismo tiempo.</br>"+
	"4. Solo existiran 4 jugadores como mínimo y máximo.</br></br>"+
	"CASILLA DE SALIDA:</br></br>"+
	"1. Cada vez que la ficha de un jugador aterriza o pasa por encima de la casilla de salida, el Banco debe pagarle al jugador # pesos.</br></br>"+
	"COMPRA DE PROPIEDADES:</br></br>"+
	"1. Si el jugador ca en una casilla que aún no ha sido comprada, tendrá la oportunidad de comprarla siempre y cuando cuente con el dinero suficiente.</br>"+
	"2. Si el jugador cae en una casilla comprada por otro jugador, deberá pagar una renta establecida en la propiedad.</br>"+
	"3. Si el jugador cae en una casilla comprada por él mismo, no se hace nada.</br></br>"+
	"CÁRCEL:</br></br>"+
	"1. Si un jugador aterriza en la casilla de la cárcel, perderá un turno.</br></br>"+
	"TARJETAS AZULES Y ROJAS:</br></br>"+
	"1. Las tarjetas Rojas representan castigos.</br>"+
	"2. Las Tarjetas azules representan premios.</br>"+
	"3. Si un jugador aterriza en un a casilla de color azul o roja, se sacará una tarjeta del respectivo color y se ejecutará el castigo o el premio mencionado en la tarjeta.</br></br></div>";

	openModal('REGLAS','Estas son las reglas:  '+rules,'ACEPTAR',null,'closeModal();',null)
}