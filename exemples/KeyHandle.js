//
// This exemple show two forms of key handle 
//

function setup(){
  SetWindowSize(800, 450)
  SetWindowTitle("KeyHandle 2 - JS")
  SetTargetFPS(30)
}

function loop(){
  ClearBackground({r: 245,g: 244, b: 245, a: 255 })

  if (IsKeyPressed(Keyboard.KEY_A)) {
    console.log("A")
  }

  var Key = GetKeyPressed();
  if (Key == Keyboard.KEY_Q){
    print("Key Q")
  }
}


