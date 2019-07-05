function init() {
    var canvas = document.getElementByTagName('canvas')[0];
    var ctx = canvas.getContext('2d');

    ctx.fillRect(0, 0, canvas.width, canvas.height);
    
    function draw(event) {
        ctx.clearRect(0, 0, canvas.width, canvas.height);
        ctx.fillStyle = 'black';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        ctx.fillStyle = 'orange';
        ctx.font = 'normal bold 2em curier';

        var x = event.clientX - canvas.getBoundingClientRect().left;
        var y = event.clientX - canvas.getBoundingClientRect().top;
        var text = x + ',' + y;
        ctx.fillText(text, 100, 100);
    }

    canvas.addEventListener('mousemove', draw, false);
}
