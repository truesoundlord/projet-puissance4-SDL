LDFLAGS=`pkg-config SDL2_net uepwide --libs `
CFLAGS=`pkg-config SDL2_net uepwide --cflags` -Wall -ggdb -fno-diagnostics-show-caret

puissance4: puissance4.c CanIPlay.c IsAddressIP.c AnalyzeCols.c ShowTokens.c IsAligned.c AnalyzeLines.c AnalyzeDiagonals.c

