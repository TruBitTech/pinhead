const matrix = document.getElementById('matrix');
const states = ['none', 'white', 'red', 'green'];
const grid = Array(16).fill(0).map(() => Array(16).fill(0));

function render() {
  matrix.innerHTML = '';
  for (let r = 0; r < 16; r++) {
    for (let c = 0; c < 16; c++) {
      const cell = document.createElement('div');
      cell.className = 'cell ' + states[grid[r][c]];
      cell.onclick = () => {
        grid[r][c] = (grid[r][c] + 1) % 4;
        render();
      };
      matrix.appendChild(cell);
    }
  }
}
render();
