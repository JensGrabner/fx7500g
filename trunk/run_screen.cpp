#include "run_screen.h"

void RunScreen::buttonClicked(int button)
{
  EditorScreen::buttonClicked(button);

  switch (button)
  {
  case Button_Exe:
    switch (_calcState->keyMode())
    {
    case KeyMode_Shift:
    case KeyMode_ShiftMode:
    case KeyMode_ShiftHyp:
      carriageReturn(); break;
    default:
      validate(); break;
    }
    break;
  default:;
  }
}

void RunScreen::validate()
{
  TextLine line("651651.", true);
  _lines << line;

  _lines << TextLine();
  _editZoneTopLineIndex = _lines.count() - 1;
  moveCursor(_editZoneTopLineIndex, 0);

  feedScreen();
  emit screenChanged();
}
