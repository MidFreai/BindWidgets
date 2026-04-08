function setup(){
  SetWindowSize(800, 600)
  SetWindowTitle("Moviment - JS")
  SetTargetFPS(30)
}

var player = {x: 350, y: 250, speed: 10, color: {r:255, g:0, b:0, a:255}};

function loop(){
  ClearBackground({r: 245, g: 245, b: 245, a: 255 })

  DrawRectangle(player.x, player.y, 100, 100, player.color)

  GetKeys().forEach(function(K){
    if (K == "KEY_A") {
      player.x -= 1 * player.speed
    }
    if (K == "KEY_D"){
      player.x += 1 * player.speed
    }
    if (K == "KEY_W"){
      player.y -= 1 * player.speed
    }
    if (K == "KEY_S"){
      player.y += 1 * player.speed
    }
  });
}

function GetKeys(){
  var key = [];
  var keyboard_array = Object.keys(Keyboard);

  keyboard_array.forEach(function(value){
    if(IsKeyDown(Keyboard[value])){
      key.push(value)
    }
  });

  return key;
}
