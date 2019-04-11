//課題1
//2019/04/12

#include <curses.h>

//ぷよの色を表すの列挙型
//NONEが無し，RED,BLUE,..が色を表す
enum puyocolor { NONE, RED, BLUE, GREEN, YELLOW };


//盤面状態
puyocolor *data = NULL;
//盤面の行数，列数
unsigned int data_line = 0;
unsigned int data_column = 0;

//メモリ開放
void Release()
{
	if (data == NULL) {
		return;
	}

	delete[] data;
	data = NULL;
}

//盤面サイズ変更
void ChangeSize(unsigned int line, unsigned int column)
{
	Release();

	//新しいサイズでメモリ確保
	data = new puyocolor[line*column];

	data_line = line;
	data_column = column;
}

//盤面の行数を返す
unsigned int GetLine()
{
	return data_line;
}

//盤面の列数を返す
unsigned int GetColumn()
{
	return data_column;
}

//盤面の指定された位置の値を返す
puyocolor GetValue(unsigned int y, unsigned int x)
{
	if (y >= GetLine() || x >= GetColumn())
	{
		//引数の値が正しくない
		return NONE;
	}

	return data[y*GetColumn() + x];
}

//盤面の指定された位置に値を書き込む
void SetValue(unsigned int y, unsigned int x, puyocolor value)
{
	if (y >= GetLine() || x >= GetColumn())
	{
		//引数の値が正しくない
		return;
	}

	data[y*GetColumn() + x] = value;
}


//盤面に新しいぷよ生成
void GeneratePuyo()
{
	puyocolor newpuyo1;
	newpuyo1 = RED;

	puyocolor newpuyo2;
	newpuyo2 = BLUE;

	SetValue(0, 5, newpuyo1);
	SetValue(0, 6, newpuyo2);
}

//ぷよの着地判定．着地判定があるとtrueを返す
bool LandingPuyo()
{
	bool landed = false;

	for (int y = 0; y < GetLine(); y++)
	{
		for (int x = 0; x < GetColumn(); x++)
		{
			if (GetValue(y, x) != NONE && y == GetLine() - 1)
			{
				landed = true;

				//着地判定されたぷよを消す．本処理は必要に応じて変更する．
				SetValue(y, x, NONE);
			}
		}
	}

	return landed;
}

//左移動
void MoveLeft()
{
	//一時的格納場所メモリ確保
	puyocolor *puyo_temp = new puyocolor[GetLine()*GetColumn()];

	for (int i = 0; i < GetLine()*GetColumn(); i++)
	{
		puyo_temp[i] = NONE;
	}

	//1つ左の位置にpuyoactiveからpuyo_tempへとコピー
	for (int y = 0; y < GetLine(); y++)
	{
		for (int x = 0; x < GetColumn(); x++)
		{
			if (GetValue(y, x) == NONE) {
				continue;
			}

			if (0 < x && GetValue(y, x - 1) == NONE)
			{
				puyo_temp[y*GetColumn() + (x - 1)] = GetValue(y, x);
				//コピー後に元位置のpuyoactiveのデータは消す
				SetValue(y, x, NONE);
			}
			else
			{
				puyo_temp[y*GetColumn() + x] = GetValue(y, x);
			}
		}
	}

	//puyo_tempからpuyoactiveへコピー
	for (int y = 0; y < GetLine(); y++)
	{
		for (int x = 0; x < GetColumn(); x++)
		{
			SetValue(y, x, puyo_temp[y*GetColumn() + x]);
		}
	}

	//一時的格納場所メモリ解放
	delete[] puyo_temp;
}

//右移動
void MoveRight()
{
	//一時的格納場所メモリ確保
	puyocolor *puyo_temp = new puyocolor[GetLine()*GetColumn()];

	for (int i = 0; i < GetLine()*GetColumn(); i++)
	{
		puyo_temp[i] = NONE;
	}

	//1つ右の位置にpuyoactiveからpuyo_tempへとコピー
	for (int y = 0; y < GetLine(); y++)
	{
		for (int x = GetColumn() - 1; x >= 0; x--)
		{
			if (GetValue(y, x) == NONE) {
				continue;
			}

			if (x < GetColumn() - 1 && GetValue(y, x + 1) == NONE)
			{
				puyo_temp[y*GetColumn() + (x + 1)] = GetValue(y, x);
				//コピー後に元位置のpuyoactiveのデータは消す
				SetValue(y, x, NONE);
			}
			else
			{
				puyo_temp[y*GetColumn() + x] = GetValue(y, x);
			}
		}
	}

	//puyo_tempからpuyoactiveへコピー
	for (int y = 0; y <GetLine(); y++)
	{
		for (int x = 0; x <GetColumn(); x++)
		{
			SetValue(y, x, puyo_temp[y*GetColumn() + x]);
		}
	}

	//一時的格納場所メモリ解放
	delete[] puyo_temp;
}

//下移動
void MoveDown()
{
	//一時的格納場所メモリ確保
	puyocolor *puyo_temp = new puyocolor[GetLine()*GetColumn()];

	for (int i = 0; i < GetLine()*GetColumn(); i++)
	{
		puyo_temp[i] = NONE;
	}

	//1つ下の位置にpuyoactiveからpuyo_tempへとコピー
	for (int y = GetLine() - 1; y >= 0; y--)
	{
		for (int x = 0; x < GetColumn(); x++)
		{
			if (GetValue(y, x) == NONE) {
				continue;
			}

			if (y < GetLine() - 1 && GetValue(y + 1, x) == NONE)
			{
				puyo_temp[(y + 1)*GetColumn() + x] = GetValue(y, x);
				//コピー後に元位置のpuyoactiveのデータは消す
				SetValue(y, x, NONE);
			}
			else
			{
				puyo_temp[y*GetColumn() + x] = GetValue(y, x);
			}
		}
	}

	//puyo_tempからpuyoactiveへコピー
	for (int y = 0; y < GetLine(); y++)
	{
		for (int x = 0; x < GetColumn(); x++)
		{
			SetValue(y, x, puyo_temp[y*GetColumn() + x]);
		}
	}

	//一時的格納場所メモリ解放
	delete[] puyo_temp;
}


//表示
void Display()
{
	//落下中ぷよ表示
	for (int y = 0; y < GetLine(); y++)
	{
		for (int x = 0; x < GetColumn(); x++)
		{
			switch (GetValue(y, x))
			{
			case NONE:
				mvaddch(y, x, '.');
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
	for (int y = 0; y < GetLine(); y++)
	{
		for (int x = 0; x < GetColumn(); x++)
		{
			if (GetValue(y, x) != NONE)
			{
				count++;
			}
		}
	}

	char msg[256];
	sprintf(msg, "Field: %d x %d, Puyo number: %03d", GetLine(), GetColumn(), count);
	mvaddstr(2, COLS - 35, msg);

	refresh();
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
	ChangeSize(LINES/2, COLS/2);	//フィールドは画面サイズの縦横1/2にする
	GeneratePuyo();	//最初のぷよ生成

	int delay = 0;
	int waitCount = 20000;

	int puyostate = 0;


	//メイン処理ループ
	while (1)
	{
		//キー入力受付
		int ch;
		ch = getch();

		//Qの入力で終了
		if (ch == 'Q')
		{
			break;
		}

		//入力キーごとの処理
		switch (ch)
		{
		case KEY_LEFT:
			MoveLeft();
			break;
		case KEY_RIGHT:
			MoveRight();
			break;
		case 'z':
			//ぷよ回転処理
			break;
		default:
			break;
		}


		//処理速度調整のためのif文
		if (delay%waitCount == 0){
			//ぷよ下に移動
			MoveDown();
			
			//ぷよ着地判定
			if (LandingPuyo())
			{
				//着地していたら新しいぷよ生成
				GeneratePuyo();
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
