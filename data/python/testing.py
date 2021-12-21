import scene_creator as sc
import random as rnd

s = sc.Scene()

for i in range(100):
	for j in range(100):
		s.add_voxel((i, 0, j), sc.LambertMaterial((rnd.random(), rnd.random(), rnd.random())))

s.moveCamera((0, -10, 0))

s.write("data/scenes/plane")