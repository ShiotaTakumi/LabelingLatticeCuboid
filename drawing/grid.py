#! /usr/bin/python3

header = """<?xml version="1.0" encoding="UTF-8"?>
<svg id="_レイヤー_1" data-name=" レイヤー 1" xmlns="http://www.w3.org/2000/svg" version="1.1" viewBox="0 0 %d %d">
  <defs>
    <style>
      .cls-1 {
        stroke: #000;
        stroke-width: .3px;
        fill: none;
      }

      .cls-2{
        fill: #008000;
        font-family: ArialMT, Arial;
        font-size: 2px;
        isolation: isolate;
      }

      .cls-3{
        fill: #ff0000;
        font-family: ArialMT, Arial;
        font-size: 2px;
        isolation: isolate;
      }

      .cls-4{
        fill: #0000ff;
        font-family: ArialMT, Arial;
        font-size: 2px;
        isolation: isolate;
      }

      .cls-5 {
        fill: #ffffff;
      }

      .cls-6 {
        fill: none;
        stroke: #231815;
        stroke-width: .5px;
      }

      .cls-7, .cls-8 {
        fill: none;
        stroke: #231815;
        stroke-width: .5px;
        stroke-miterlimit: 10;
      }

	  .cls-8 {
	    stroke-dasharray: 0 0 1 1;
	  }
    </style>
  </defs>
"""
square = """<rect class="cls-1" x="%d" y="%d" width="10" height="10"/>
"""
face_No = """<text class="cls-2" x="%d" y="%d" text-anchor="middle" dominant-baseline="central">%d</text>
""" 
edge_No = """<text class="cls-3" x="%d" y="%d" text-anchor="middle" dominant-baseline="central">%d</text>
"""
vertex_No = """<text class="cls-4" x="%d" y="%d" text-anchor="middle" dominant-baseline="central">%d</text>
"""
white = """<rect class="cls-5" x="%d" y="%d" width="2" height="2"/>
"""
line = """<line class="cls-6" x1="%d" y1="%d" x2="%d" y2="%d"/>
"""
dash_line = """<line class="cls-8" x1="%d" y1="%d" x2="%d" y2="%d"/>
"""
footer = """</svg>
"""

def main():
	a = int(input('a: '))
	b = int(input('b: '))
	x = int(input('x: '))
	y = int(input('y: '))
	z = int(input('z: '))
	wl = 1 # 横幅用（左）1列
	wr = 1 # 横幅用（右）1列
	ht = 1 # 高さ用（上）1列
	hd = 1 # 高さ用（下）1列
	height = ht + (b * y) + (a * x) + (a * z) + (a * x) + (b * x) + (a * z) + hd
	width = wl + (a * y) + (b * x) + (b * z) + (b * x) + (a * x) + (b * z) + wr
	margin = 10 # ブラウザ上で表示する際のマージン
	scale = 10 # 1マスのサイズ
	hsft = 0 # 画面外に出ないよう，下にシフト
	if (a * x) + (a * z) + (a * x) < (b * x):
		hsft = (b * x) - ((a * x) + (a * z) + (a * x))
	output_filename = f"out.svg"
	cnt = 0
	"""
	*********************************** 
	 * 大枠で見た時の面の番号は下記の通り
	 * 
	 *          4
	 *    0 1 2 3
	 *          5
	 * 
	************************************
	"""
	with open(output_filename, 'w') as out_file:
		# SVG のヘッダーを記入
		out_file.write(header %(margin + width * scale + margin, margin + (hsft + height) * scale + margin))
		# 格子を描画
		for i in range(0, height + hsft):
			for j in range(0, width):
				out_file.write(square %(margin + j * scale, margin + i * scale))
		# 0 1 2 面の左右の辺を描画
		for i in range(0, x + z + x + 1):
			x1 = margin + (wl + (a * y) + b * i) * scale
			y1 = margin + (ht + hsft + a * i) * scale
			x2 = margin + (wl + b * i) * scale
			y2 = margin + (ht + hsft + a * i + (b * y)) * scale
			if i == 0 or i == x or i == x + z or i == x + z + x or i == x + z + x + z:
				out_file.write(line %(x1, y1, x2, y2))
			else:
				out_file.write(dash_line %(x1, y1, x2, y2))
		# 0 1 2 面の上下の辺を描画
		for i in range(0, y + 1):
			x1 = margin + (wl + (a * y) - a * i) * scale
			y1 = margin + (ht + hsft + b * i) * scale
			x2 = margin + (wl + (a * y) + (x + z + x) * b - a * i) * scale
			y2 = margin + (ht + hsft + (x + z + x) * a + b * i) * scale
			if i == 0 or i == y:
				out_file.write(line %(x1, y1, x2, y2))
			else:
				out_file.write(dash_line %(x1, y1, x2, y2))
		# 3 4 5 面の上下の辺を描画
		for i in range(0, x + y + x + 1):
			x1 = margin + (wl + (a * y) + (x + z + x) * b + (a * x) - a * i) * scale
			y1 = margin + (ht + hsft + (x + z + x) * a - (b * x) + b * i) * scale
			x2 = margin + (wl + (a * y) + (x + z + x) * b + (a * x) + (b * z) - a * i) * scale
			y2 = margin + (ht + hsft + (x + z + x) * a - (b * x) + 1 * (a * z) + b * i) * scale
			if i == 0 or i == x or i == x + y or i == x + y + x:
				out_file.write(line %(x1, y1, x2, y2))
			else:
				out_file.write(dash_line %(x1, y1, x2, y2))
		# 3 4 5 面の左右の辺を描画
		for i in range(0, z + 1):
			x1 = margin + (wl + (a * y) + (x + z + x) * b + (a * x) + b * i) * scale
			y1 = margin + (ht + hsft + (x + z + x) * a - (b * x) + a * i) * scale
			x2 = margin + (wl + (a * y) + (x + z + x) * b - (a * (y + x)) + b * i) * scale
			y2 = margin + (ht + hsft + (x + z + x) * a + (b * (y + x)) + a * i) * scale
			if i == 0 or i == z:
				out_file.write(line %(x1, y1, x2, y2))
			else:
				out_file.write(dash_line %(x1, y1, x2, y2))

		for i in range(0, height + hsft):
			for j in range(0, width):
				# 面番号を描画
				x1 = margin + (j + 0.5) * 10
				y1 = margin + (i + 0.5) * 10
				out_file.write(face_No %(x1, y1, cnt))
				# 水平方向の辺番号を描画 (white における -1 は位置調整用)
				x1 = margin + (j + 0.5) * 10
				y1 = margin + i * 10
				out_file.write(white %(x1 - 1, y1 - 1))
				out_file.write(edge_No %(x1, y1, 2 * cnt))
				# 垂直方向の辺番号を描画 (white における -1 は位置調整用)
				x1 = margin + j * 10
				y1 = margin + (i + 0.5) * 10
				out_file.write(white %(x1 - 1, y1 - 1))
				out_file.write(edge_No %(x1, y1, 2 * cnt + 1))
				# 頂点番号を描画 (white における -1 は位置調整用)
				x1 = margin + j * 10
				y1 = margin + i * 10
				out_file.write(white %(x1 - 1, y1 - 1))
				out_file.write(vertex_No %(x1, y1, cnt))
				cnt += 1

		# SVG のフッターを記入
		out_file.write(footer)
			
####################
if __name__ == "__main__":
	main()
