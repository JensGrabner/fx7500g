#include "prog_edit_screen.h"

void ProgEditScreen::buttonClicked(int button)
{
  EditorScreen::buttonClicked(button);

  switch (button)
  {
  case Button_Exe: carriageReturn(); break;
  default:;
  }
}
