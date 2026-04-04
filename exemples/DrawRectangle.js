function setup(){
  SetWindowSize(800, 600)
  SetWindowTitle("DrawRectangle - JS")
  SetTargetFPS(30)
}

function loop(){
  ClearBackground({a: 255 })
  DrawRectangle(350, 250, 100, 100, {r: 255, a:255})
}

