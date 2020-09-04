import win32file
from typing import Any, Iterator
import time

PIPE_NAME = '\\\\.\\pipe\\GDPipe'
CHUNK_SIZE = 5


def chunks(object_list: list, size: int) -> Iterator[Any]:

	for i in range(0, len(object_list), size):

		yield object_list[i:i + size]

		
def draw_object(level_object) -> None:

	handle = win32file.CreateFile (
		PIPE_NAME,
		win32file.GENERIC_READ | win32file.GENERIC_WRITE,
		0, None,
		win32file.OPEN_EXISTING,
		0, None
	)

	for object_list in chunks(level_object.block_data, CHUNK_SIZE):
		object_string = ';'.join([str(x) for x in object_list]) + ';'
		win32file.WriteFile(handle, object_string.encode())
		time.sleep(0.05)