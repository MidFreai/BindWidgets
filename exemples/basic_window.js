function setup(){
  SetWindowSize(800, 450)
  SetWindowTitle("Test 1 - JS")
}

function loop() {
  ClearBackground({r: 245, g: 245, b: 245, a: 255 })
  DrawText("Text inside js", 200, 100, 30)
  DrawText("Another text", 200, 300, 10)
}

