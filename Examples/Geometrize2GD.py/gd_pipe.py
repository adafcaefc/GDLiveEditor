import win32file

from typing import Any, Iterator

from time import sleep

import colorama
from colorama import Fore, Style, Cursor

from textwrap import indent, dedent

PIPE_NAME = '\\\\.\\pipe\\GDPipe'
CHUNK_SIZE = 5


def chunks(object_list: list, size: int) -> Iterator[Any]:

	for i in range(0, len(object_list), size):

		yield object_list[i:i + size]

		
def draw_object(level_object) -> None:

	handle = win32file.CreateFile(PIPE_NAME, win32file.GENERIC_READ | win32file.GENERIC_WRITE, 0, None, win32file.OPEN_EXISTING, 0, None)

	blocks_drawn = 0

	for object_list in chunks(level_object.block_data, CHUNK_SIZE):

		print(indent(dedent(
			f'''
			{Fore.GREEN}Drawing blocks ({blocks_drawn}/{len(level_object.block_data)})
			'''
		), " "))

		object_string = ';'.join([str(x) for x in object_list]) + ';'
		win32file.WriteFile(handle, object_string.encode())

		blocks_drawn += CHUNK_SIZE

		sleep(0.05)

		print((Cursor.UP() + colorama.ansi.clear_line()) * 3, end='')
