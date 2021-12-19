#include "kGui.h"

Component create_grid(int cols, int rows, int cellWidth, int cellHeight) {
	Component component;
	component.type = 0;
	component.data.grid.cols = cols;
	component.data.grid.rows = rows;
	component.data.grid.cellWidth = cellWidth;
	component.data.grid.cellHeight = cellHeight;
	component.data.grid.components = malloc(sizeof(Component) * cols * rows);

	for (int i = 0; i < cols * rows; i++) {
		component.data.grid.components[i] = -1;
	}

	return component;
}

void _draw_grid(Grid *grid, int x, int y) {
	for (int i = 0; i < grid->rows; i++) {
		for (int j = 0; j < grid->cols; j++) {
			_draw_component(grid->components[j + i * grid->cols], x + j * grid->cellWidth + kGS.settings.padding,
							y + i * grid->cellHeight + kGS.settings.padding);
		}
	}
}

void _process_grid(Grid *grid, int x, int y) {
	for (int i = 0; i < grid->rows; i++) {
		for (int j = 0; j < grid->cols; j++) {
			_process_component(grid->components[j + i * grid->cols], x + j * grid->cellWidth + kGS.settings.padding,
							   y + i * grid->cellHeight + kGS.settings.padding);
		}
	}
}