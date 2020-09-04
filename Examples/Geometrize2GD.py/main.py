import json

from colorsys import rgb_to_hsv
from typing import Any, Dict, List

import colorama
from colorama import Fore, Style, Cursor

from textwrap import indent, dedent

from gd_object import gd_object_collection
from gd_pipe import draw_object

from time import sleep

__program_name__ = 'Geometrize2GD.py'
__author__ = 'Adafcaefc'
__version__ = '1.1.0'

DRAW_SCALE = 1
CIRCLE_TYPE = [32, 5]
CIRCLE_ID = 1764
THRESHOLD = 0
TOLERANCE = 0
CHROMA_KEY = [0,0,0]

OBJECT_LIST = gd_object_collection()


def draw_json_to_gd(parsed_json: Dict[str, Any], opaque: bool=True) -> None:

	z_order = 0

	for shape in parsed_json:

		if (shape["type"] in CIRCLE_TYPE) and (shape["score"] > THRESHOLD):
			if (
				not all(
					(
						-int(TOLERANCE) <= (shape["color"][i] - int(CHROMA_KEY[i])) <= int(TOLERANCE)
					) for i in range(3)
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

	global DRAW_SCALE, THRESHOLD, TOLERANCE, CHROMA_KEY, OBJECT_LIST
	
	print(indent(dedent(
		f'''
		{Fore.CYAN}{__program_name__} v{__version__} by {__author__}

		{Fore.MAGENTA}Have fun, happy creating!

		{Fore.RED}Make sure that you have GD Live Editor installed!'''
	), " "))

	while True:

		OBJECT_LIST = gd_object_collection()

		print(indent(dedent(
			f'''
			{Fore.YELLOW}Enter your geometrize JSON file name.{Fore.GREEN}
			'''
		), " "))

		json_file = False
		
		while not json_file:
			json_path = input(" ")
			try:
				json_file = open(json_path, 'r').read()
			except:
				try:
					json_file = open(json_path + '.json', 'r').read()
				except:
					clear_line(2)

					print(indent(dedent(
						f'''
						{Fore.RED}File not found!{Fore.GREEN}
						'''
					), " "))

					sleep(1)
					clear_line(2)

		parsed_json = json.loads(json_file)

		clear_line(4)

		print(indent(dedent(
			f'''
			{Fore.YELLOW}Enter image scale.{Fore.GREEN}
			'''
		), " "))

		DRAW_SCALE = 1 / float(input(" "))

		clear_line(4)

		print(indent(dedent(
			f'''
			{Fore.YELLOW}Enter chroma key filter in {Fore.GREEN}r,g,b{Fore.YELLOW} format, fill 0 to use the opaque mode.{Fore.GREEN}
			'''
		), " "))

		chroma_key_input = input(" ").split(',')

		clear_line(4)

		is_opaque = not (len(chroma_key_input) == 3)

		if not is_opaque:

			CHROMA_KEY = chroma_key_input

			print(indent(dedent(
				f'''
				{Fore.YELLOW}Enter chroma key strength between {Fore.GREEN}0, 255{Fore.GREEN}
				'''
			), " "))

			TOLERANCE = input(" ")

			clear_line(4)
		
		draw_json_to_gd(parsed_json, is_opaque)



def clear_line(number_of_lines: int) -> None:

  print((Cursor.UP() + colorama.ansi.clear_line()) * number_of_lines, end='')


if __name__ == '__main__':

	colorama.init(autoreset=False)
	print(Style.BRIGHT, end='')

	main()