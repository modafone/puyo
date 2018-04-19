#include <curses.h>


//ぷよの色を表すの列挙型
//NONEが無し，RED,BLUE,..が色を表す
enum puyocolor{NONE, RED, BLUE, GREEN, YELLOW};

//ぷよ1つのデータを格納する構造体
struct puyodata
{
	enum puyocolor color;	//色
	bool handling;	//操作中か否か

	//構造体のコンストラクタ
	//この構造体のインスタンス定義時に一度だけ実行される関数
	//ここでは変数の値を初期化する
	puyodata()
	{
		color = NONE;
		handling = false;
	}
};


//フィールドを表す2次元配列
struct puyodata **field_array = NULL;
//フィールドの行数，列数
unsigned int field_line = 0;
unsigned int field_column = 0;

//field_arrayのメモリ開放
void Field_Release()
{
	if (field_array == NULL)
	{
		return;
	}

	for (int y = 0; y < field_line; y++)
	{
		delete[] field_array[y];
		field_array[y] = NULL;
	}
	delete[] field_array;
	field_array = NULL;
}


//フィールドサイズ変更
void Field_ChangeSize(unsigned int line, unsigned int column)
{
	Field_Release();
		
	//新しいサイズでメモリ確保
	field_array = new struct puyodata*[line];
	for (int y = 0; y < line; y++)
	{
		field_array[y] = new struct puyodata[column];
	}

	field_line = line;
	field_column = column;
}

//フィールドの指定された位置の値を返す
struct puyodata Field_GetValue(unsigned int y, unsigned int x)
{
	if (y >= field_line || x >= field_column)
	{
		//引数の値が正しくない
		struct puyodata temp;
		return temp;
	}

	return field_array[y][x];
}

//フィールドの指定された位置に値を書き込む
void Field_SetValue(unsigned int y, unsigned int x, struct puyodata data)
{
	if (y >= field_line || x >= field_column)
	{
		//引数の値が正しくない
		return;
	}

	field_array[y][x] = data;
}


//フィールドに新しいぷよ生成
void Field_GeneratePuyo()
{
	field_array[0][5].color = RED;
	field_array[0][5].handling = true;

	field_array[0][6].color = BLUE;
	field_array[0][6].handling = true;
}

//操作中ぷよの着地判定
//着地判定があるとtrueを返す
bool Field_LandingPuyo()
{
	bool landed = false;

	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (field_array[y][x].handling == true)
			{
				if (y == field_line - 1 || field_array[y+1][x].color != NONE)
				{
					//操作中フラグをfalseにして着地状態にする
					field_array[y][x].handling = false;
					//左右のぷよも着地状態にする
					field_array[y][x-1].handling = false;
					field_array[y][x+1].handling = false;

					landed = true;
				}
			}
		}
	}

	return landed;
}

//下移動
void Field_ControlDown()
{
	//一時的格納場所メモリ確保
	struct puyodata **field_array_temp;

	field_array_temp = new struct puyodata*[field_line];
	for (int y = 0; y < field_line; y++)
	{
		field_array_temp[y] = new struct puyodata[field_column];
	}

	//1つ下の位置にfield_arrayからfield_array_tempへとコピー
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (field_array[y][x].handling == true)
			{
				if (y < field_line-1)
				{
					field_array_temp[y+1][x] = field_array[y][x];
				}
				else
				{
					field_array_temp[y][x] = field_array[y][x];
				}

				//コピー後にfield_arrayのデータは消す
				field_array[y][x].color = NONE;
				field_array[y][x].handling = false;
			}
		}
	}

	//field_array_tempからfield_arrayへコピー
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (field_array_temp[y][x].handling == true)
			{
				field_array[y][x] = field_array_temp[y][x];
			}
		}
	}

	//一時的格納場所メモリ解放
	for (int y = 0; y < field_line; y++)
	{
		delete[] field_array_temp[y];
	}
	delete[] field_array_temp;
}

//左移動
void Field_ControlLeft()
{
	//一時的格納場所メモリ確保
	struct puyodata **field_array_temp;
		
	field_array_temp = new struct puyodata*[field_line];
	for (int y = 0; y < field_line; y++)
	{
		field_array_temp[y] = new struct puyodata[field_column];
	}

	//1つ左の位置にfield_arrayからfield_array_tempへとコピー
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (field_array[y][x].handling == true)
			{
				if (0 < x)
				{
					field_array_temp[y][x-1] = field_array[y][x];
				} 
				else
				{
					field_array_temp[y][x] = field_array[y][x];
				}

				//コピー後にfield_arrayのデータは消す
				field_array[y][x].color = NONE;
				field_array[y][x].handling = false;
			}
		}
	}

	//field_array_tempからfield_arrayへコピー
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (field_array_temp[y][x].handling == true)
			{
				field_array[y][x] = field_array_temp[y][x];
			}
		}
	}

	//一時的格納場所メモリ解放
	for (int y = 0; y < field_line; y++)
	{
		delete[] field_array_temp[y];
	}
	delete[] field_array_temp;
}

//右移動
void Field_ControlRight()
{
	//一時的格納場所メモリ確保
	struct puyodata **field_array_temp;

	field_array_temp = new struct puyodata*[field_line];
	for (int y = 0; y < field_line; y++)
	{
		field_array_temp[y] = new struct puyodata[field_column];
	}

	//1つ右の位置にfield_arrayからfield_array_tempへとコピー
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (field_array[y][x].handling == true)
			{
				if (x <= field_column)
				{
					field_array_temp[y][x + 1] = field_array[y][x];
				}
				else
				{
					field_array_temp[y][x] = field_array[y][x];
				}

				//コピー後にfield_arrayのデータは消す
				field_array[y][x].color = NONE;
				field_array[y][x].handling = false;
			}
		}
	}

	//field_array_tempからfield_arrayへコピー
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (field_array_temp[y][x].handling == true)
			{
				field_array[y][x] = field_array_temp[y][x];
			}
		}
	}

	//一時的格納場所メモリ解放
	for (int y = 0; y < field_line; y++)
	{
		delete[] field_array_temp[y];
	}
	delete[] field_array_temp;
}


//表示
void Display()
{
	//フィールド表示
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			switch (Field_GetValue(y, x).color)
			{
			case NONE:
				mvaddch(y, x, ' ');
				break;
			case RED:
				mvaddch(y, x, 'R');
				break;
			case BLUE:
				mvaddch(y, x, 'B');
				break;
			case GREEN:
				mvaddch(y, x, 'G');
				break;
			case YELLOW:
				mvaddch(y, x, 'Y');
				break;
			default:
				mvaddch(y, x, '?');
				break;
			}
			
		}
	}


	//情報表示
	int count = 0;
	for (int y = 0; y < field_line; y++)
	{
		for (int x = 0; x < field_column; x++)
		{
			if (Field_GetValue(y, x).color != NONE)
			{
				count++;
			}
		}
	}

	char msg[256];
	sprintf(msg, "Field: %d x %d, Puyo number: %03d", field_line, field_column, count);
	mvaddstr(2, COLS - 35, msg);
}


//ここから実行される
int main(int argc, char **argv){
	//画面の初期化
	initscr();
	//カラー属性を扱うための初期化
	start_color();

	//キーを押しても画面に表示しない
	noecho();
	//キー入力を即座に受け付ける
	cbreak();

	curs_set(0);
	//キー入力受付方法指定
	keypad(stdscr, TRUE);

	//キー入力非ブロッキングモード
	timeout(0);


	//初期化処理
	Field_ChangeSize(LINES/2, COLS/2);	//フィールドは画面サイズの縦横1/2にする
	Field_GeneratePuyo();	//最初のぷよ生成

	int delay = 0;
	int waitCount = 20000;

	while (1)
	{
		//キー入力受付
		int ch;
		ch = getch();

		if (ch == 'Q')
		{
			break;
		}

		switch (ch)
		{
		case KEY_LEFT:
			Field_ControlLeft();
			break;
		case KEY_RIGHT:
			Field_ControlRight();
			break;
		case 'Z':
			//ぷよ回転処理など
			break;
		default:
			break;
		}


		//処理速度調整のためのif文
		if (delay%waitCount == 0){
			//ぷよ下に移動
			Field_ControlDown();
			
			//ぷよ着地判定
			if (Field_LandingPuyo()){
				//着地したら新しいぷよ生成
				Field_GeneratePuyo();
			}
		}
		delay++;


		//表示
		Display();
	}


	//画面をリセット
	endwin();

	return 0;
}
