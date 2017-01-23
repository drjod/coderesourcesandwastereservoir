

var width = 50;
var height = 50;

function Playground(width, height) {
	this.width = width;
	this.height = height;

	this.cells = [];
	for (var i = 0; i < this.width; i++) {
		var subcell = [];
		for(var j = 0; j < this.height; j++) {
			subcell.push(0);	
		}
		this.cells.push(subcell);
	}

        this.canvas = document.getElementById("playground");
        this.context = this.canvas.getContext("2d");
	this.context.fillStyle="FF000"


	this.draw_canvas = function() {
		this.context.clearRect(0, 0, 10 * this.width, 10 * this.height)
		for(var i = 0; i < this.width; i++) {
			for(var j = 0; j< this.height; j++) {
				if(this.cells[i][j] === 1) { 
       					this.context.fillRect(i * 10, j * 10, 10, 10);
				}
			}
		}
	}

	this.is_alive = function() {
		for(var i = 0; i < this.width; i++) {
			for(var j = 0; j < this.height; j++) {
				if (this.cells[i][j] === 1) {
					return true;
				}
			}
		}
		return false;
	};

	this.determine_new_cell_status = function(i, j) {
		var neighbors = this.determine_neighbors(i, j);

		var number_of_neighbors_alive = 0;
		
		for (var k = 0; k < neighbors.length; k++) {
			number_of_neighbors_alive += this.cells[neighbors[k][0]][neighbors[k][1]];
		}
		if (number_of_neighbors_alive === 3) {
			return 1;
		}

		var old_status = this.cells[i][j];
		if(number_of_neighbors_alive === 2 && old_status === 1) {
			return 1;
		}

		return 0;
	};

	this.determine_neighbors = function(i, j) {
		result = [];
		result.push([(i - 1 + this.width) % this.width, (j - 1 + this.height) % this.height]);
		result.push([(i - 1 + this.width) % this.width, j]);
		result.push([(i - 1 + this.width) % this.width, (j + 1 ) % this.height]);

		result.push([i, (j - 1 + this.height) % this.height]);
		result.push([i, (j + 1) % this.height]);

		result.push([(i + 1) % this.width, (j - 1 + this.height) % this.height]);
		result.push([(i + 1) % this.width,j]);
		result.push([(i + 1) % this.width, (j + 1) % this.height]);

		return result;
	};

	this.determine_new_playground = function() {
		var playground = new Playground(this.width, this.height);
		
		for(var i = 0; i < this.width; i++) {
			for(var j = 0; j < this.height; j++) {
				playground.cells[i][j] = this.determine_new_cell_status(i, j); 
			}
		}
		return playground;
	}

        this.change_cell_status = function(i, j) {

                if(this.cells[i][j] === 0) {
                        this.cells[i][j] = 1;
                        this.context.fillRect(i * 10, j * 10, 10, 10);

                }
                else {
                        this.cells[i][j] = 0;
                	this.context.clearRect(i * 10, j * 10, 10, 10);
                }
        }



}

var playground = new Playground(width, height);
playground.draw_canvas();

function update_playground() {
	var playground_new = playground.determine_new_playground();
	playground = playground_new		

	playground.draw_canvas();
	delete playground_new;
}

/* canvas  */
 
function getMousePos(canvas, evt) {
    var rect = canvas.getBoundingClientRect();
    return {
      x: evt.clientX - rect.left,
      y: evt.clientY - rect.top
    };
  }

var mousePos; 
playground.canvas.addEventListener('mousemove', function(evt) {
    mousePos = getMousePos(playground.canvas, evt);
    /*console.log('Mouse position: ' + mousePos.x + ',' + mousePos.y);*/
  }, false);

playground.canvas.addEventListener("click", change_cell_status);

function change_cell_status() {
	var i = Math.floor(mousePos.x / 10);
	var j = Math.floor(mousePos.y / 10);

	playground.change_cell_status(i, j);
}


/* movie */

var interval_variable;

function march_through_steps() {
	document.getElementById("start_button").style.visibility = "hidden";
	document.getElementById("stop_button").style.visibility = "visible";
	interval_variable = setInterval(update_playground, 1000);

}

function stop_marching(){
	clearInterval(interval_variable);
	document.getElementById("start_button").style.visibility = "visible";
	document.getElementById("stop_button").style.visibility = "hidden";

}

document.getElementById("body").onload = function() {
	document.getElementById("stop_button").style.visibility = "hidden";
};
