from colorsys import rgb_to_hsv
from typing import Any, Dict, List
import json
from gdlevel import gd_object_collection
from gdpipe import draw_object


DRAW_SCALE = 1
CIRCLE_TYPE = [32, 5]
CIRCLE_ID = 1764
THRESHOLD = 0
TOLERANCE = 0
CHROMA_KEY = [0,0,0]

OBJECT_LIST = gd_object_collection()


def convert_json_to_gd_object_list(parsed_json: Dict[str, Any], opaque: bool=True) -> List:

	z_order = 0

	for shape in parsed_json:

		if (shape["type"] in CIRCLE_TYPE) and (shape["score"] > THRESHOLD):
			if (
				not (
					all (
						shape["color"][i] in range(CHROMA_KEY[i] - TOLERANCE, CHROMA_KEY[i] + TOLERANCE) for i in range(3)
					)
				)
			) or opaque:

				(x, y, scale, *_), (r, g, b, *_) = shape["data"], shape["color"]

				h, s, v = rgb_to_hsv(r / 255, g / 255, b / 255)

				hsv_string = f'{h * 360}a{s}a{v}a1a1'

				OBJECT_LIST.add_block (
						ID = CIRCLE_ID, 
						X = x / DRAW_SCALE, 
						Y = y / DRAW_SCALE, 
						SCALE = scale / DRAW_SCALE / 4, 
						COLOR_1_HSV_ENABLED = 1, 
						COLOR_2_HSV_ENABLED = 1, 
						COLOR_1_HSV_VALUES = hsv_string, 
						COLOR_2_HSV_VALUES = hsv_string, 
						Z_ORDER = z_order
					)

				z_order += 1

	draw_object(OBJECT_LIST)
	

def main() -> None:

	global DRAW_SCALE, THRESHOLD, TOLERANCE, CHROMA_KEY

	parsed_json = json.loads(
		open(input("Enter your geometrize JSON file name "), 'r').read())

	convert_json_to_gd_object_list(parsed_json)


if __name__ == '__main__':
	main()