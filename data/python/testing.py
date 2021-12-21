import scene_creator as sc

s = sc.Scene()

for i in range(100):
	for j in range(100):
		s.add_voxel((i, 0, j), sc.LambertMaterial((i / 100, j / 100, 0)))

s.moveCamera((0, -10, 0))

s.write("data/scenes/plane")