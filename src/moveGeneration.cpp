/*
Copyright 2019 Tyler Behme

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <moveGeneration.h>

const U64 RayTable [64][8] = {
  {0ULL, 0ULL, 0ULL, 0ULL, 9151314442816847872ULL, 36170086419038336ULL, 0ULL, 18049651735527937ULL},
  {0ULL, 0ULL, 0ULL, 9223372036854775808ULL, 4539628424389459968ULL, 18085043209519168ULL, 36028797018963968ULL, 9024825867763968ULL},
  {0ULL, 0ULL, 0ULL, 13835058055282163712ULL, 2233785415175766016ULL, 9042521604759584ULL, 18155135997837312ULL, 4512412933881856ULL},
  {0ULL, 0ULL, 0ULL, 16140901064495857664ULL, 1080863910568919040ULL, 4521260802379792ULL, 9078117754732544ULL, 2256206466908160ULL},
  {0ULL, 0ULL, 0ULL, 17293822569102704640ULL, 504403158265495552ULL, 2260630401189896ULL, 4539061024849920ULL, 1128103225065472ULL},
  {0ULL, 0ULL, 0ULL, 17870283321406128128ULL, 216172782113783808ULL, 1130315200594948ULL, 2269530520813568ULL, 564049465049088ULL},
  {0ULL, 0ULL, 0ULL, 18158513697557839872ULL, 72057594037927936ULL, 565157600297474ULL, 1134765260439552ULL, 281474976710656ULL},
  {0ULL, 0ULL, 0ULL, 18302628885633695744ULL, 0ULL, 282578800148737ULL, 567382630219904ULL, 0ULL},
  {9223372036854775808ULL, 4611686018427387904ULL, 0ULL, 0ULL, 35747322042253312ULL, 141289400074368ULL, 0ULL, 70506452091906ULL},
  {4611686018427387904ULL, 2305843009213693952ULL, 9223372036854775808ULL, 36028797018963968ULL, 17732923532771328ULL, 70644700037184ULL, 140737488355328ULL, 35253226045953ULL},
  {2305843009213693952ULL, 1152921504606846976ULL, 4611686018427387904ULL, 54043195528445952ULL, 8725724278030336ULL, 35322350018592ULL, 70918499991552ULL, 17626613022976ULL},
  {1152921504606846976ULL, 576460752303423488ULL, 2305843009213693952ULL, 63050394783186944ULL, 4222124650659840ULL, 17661175009296ULL, 35461397479424ULL, 8813306511360ULL},
  {576460752303423488ULL, 288230376151711744ULL, 1152921504606846976ULL, 67553994410557440ULL, 1970324836974592ULL, 8830587504648ULL, 17730707128320ULL, 4406653222912ULL},
  {288230376151711744ULL, 144115188075855872ULL, 576460752303423488ULL, 69805794224242688ULL, 844424930131968ULL, 4415293752324ULL, 8865353596928ULL, 2203318222848ULL},
  {144115188075855872ULL, 72057594037927936ULL, 288230376151711744ULL, 70931694131085312ULL, 281474976710656ULL, 2207646876162ULL, 4432676798592ULL, 1099511627776ULL},
  {72057594037927936ULL, 0ULL, 144115188075855872ULL, 71494644084506624ULL, 0ULL, 1103823438081ULL, 2216338399296ULL, 0ULL},
  {9259400833873739776ULL, 2323857407723175936ULL, 0ULL, 0ULL, 139637976727552ULL, 551911719040ULL, 0ULL, 275415828484ULL},
  {4629700416936869888ULL, 1161928703861587968ULL, 36028797018963968ULL, 140737488355328ULL, 69269232549888ULL, 275955859520ULL, 549755813888ULL, 137707914242ULL},
  {2314850208468434944ULL, 580964351930793984ULL, 9241386435364257792ULL, 211106232532992ULL, 34084860461056ULL, 137977929760ULL, 277025390592ULL, 68853957121ULL},
  {1157425104234217472ULL, 290482175965396992ULL, 4620693217682128896ULL, 246290604621824ULL, 16492674416640ULL, 68988964880ULL, 138521083904ULL, 34426978560ULL},
  {578712552117108736ULL, 145241087982698496ULL, 2310346608841064448ULL, 263882790666240ULL, 7696581394432ULL, 34494482440ULL, 69260574720ULL, 17213489152ULL},
  {289356276058554368ULL, 72620543991349248ULL, 1155173304420532224ULL, 272678883688448ULL, 3298534883328ULL, 17247241220ULL, 34630287488ULL, 8606711808ULL},
  {144678138029277184ULL, 281474976710656ULL, 577586652210266112ULL, 277076930199552ULL, 1099511627776ULL, 8623620610ULL, 17315143744ULL, 4294967296ULL},
  {72339069014638592ULL, 0ULL, 288793326105133056ULL, 279275953455104ULL, 0ULL, 4311810305ULL, 8657571872ULL, 0ULL},
  {9259541571362095104ULL, 1161999072605765632ULL, 0ULL, 0ULL, 545460846592ULL, 2155905152ULL, 0ULL, 1075843080ULL},
  {4629770785681047552ULL, 580999536302882816ULL, 140737488355328ULL, 549755813888ULL, 270582939648ULL, 1077952576ULL, 2147483648ULL, 537921540ULL},
  {2314885392840523776ULL, 290499768151441408ULL, 36099165763141632ULL, 824633720832ULL, 133143986176ULL, 538976288ULL, 1082130432ULL, 268960770ULL},
  {1157442696420261888ULL, 145249884075720704ULL, 9241421619736346624ULL, 962072674304ULL, 64424509440ULL, 269488144ULL, 541097984ULL, 134480385ULL},
  {578721348210130944ULL, 72624942037860352ULL, 4620710809868173312ULL, 1030792151040ULL, 30064771072ULL, 134744072ULL, 270549120ULL, 67240192ULL},
  {289360674105065472ULL, 283673999966208ULL, 2310355404934086656ULL, 1065151889408ULL, 12884901888ULL, 67372036ULL, 135274560ULL, 33619968ULL},
  {144680337052532736ULL, 1099511627776ULL, 1155177702467043328ULL, 1082331758592ULL, 4294967296ULL, 33686018ULL, 67637280ULL, 16777216ULL},
  {72340168526266368ULL, 0ULL, 577588851233521664ULL, 1090921693184ULL, 0ULL, 16843009ULL, 33818640ULL, 0ULL},
  {9259542121117908992ULL, 580999811180789760ULL, 0ULL, 0ULL, 2130706432ULL, 8421504ULL, 0ULL, 4202512ULL},
  {4629771060558954496ULL, 290499905590394880ULL, 549755813888ULL, 2147483648ULL, 1056964608ULL, 4210752ULL, 8388608ULL, 2101256ULL},
  {2314885530279477248ULL, 145249952795197440ULL, 141012366262272ULL, 3221225472ULL, 520093696ULL, 2105376ULL, 4227072ULL, 1050628ULL},
  {1157442765139738624ULL, 72624976397598720ULL, 36099303202095104ULL, 3758096384ULL, 251658240ULL, 1052688ULL, 2113664ULL, 525314ULL},
  {578721382569869312ULL, 283691179835392ULL, 9241421688455823360ULL, 4026531840ULL, 117440512ULL, 526344ULL, 1056832ULL, 262657ULL},
  {289360691284934656ULL, 1108101562368ULL, 4620710844227911680ULL, 4160749568ULL, 50331648ULL, 263172ULL, 528416ULL, 131328ULL},
  {144680345642467328ULL, 4294967296ULL, 2310355422113955840ULL, 4227858432ULL, 16777216ULL, 131586ULL, 264208ULL, 65536ULL},
  {72340172821233664ULL, 0ULL, 1155177711056977920ULL, 4261412864ULL, 0ULL, 65793ULL, 132104ULL, 0ULL},
  {9259542123265392640ULL, 290499906664136704ULL, 0ULL, 0ULL, 8323072ULL, 32896ULL, 0ULL, 16416ULL},
  {4629771061632696320ULL, 145249953332068352ULL, 2147483648ULL, 8388608ULL, 4128768ULL, 16448ULL, 32768ULL, 8208ULL},
  {2314885530816348160ULL, 72624976666034176ULL, 550829555712ULL, 12582912ULL, 2031616ULL, 8224ULL, 16512ULL, 4104ULL},
  {1157442765408174080ULL, 283691314053120ULL, 141012903133184ULL, 14680064ULL, 983040ULL, 4112ULL, 8256ULL, 2052ULL},
  {578721382704087040ULL, 1108168671232ULL, 36099303470530560ULL, 15728640ULL, 458752ULL, 2056ULL, 4128ULL, 1026ULL},
  {289360691352043520ULL, 4328521728ULL, 9241421688590041088ULL, 16252928ULL, 196608ULL, 1028ULL, 2064ULL, 513ULL},
  {144680345676021760ULL, 16777216ULL, 4620710844295020544ULL, 16515072ULL, 65536ULL, 514ULL, 1032ULL, 256ULL},
  {72340172838010880ULL, 0ULL, 2310355422147510272ULL, 16646144ULL, 0ULL, 257ULL, 516ULL, 0ULL},
  {9259542123273781248ULL, 145249953336262656ULL, 0ULL, 0ULL, 32512ULL, 128ULL, 0ULL, 64ULL},
  {4629771061636890624ULL, 72624976668131328ULL, 8388608ULL, 32768ULL, 16128ULL, 64ULL, 128ULL, 32ULL},
  {2314885530818445312ULL, 283691315101696ULL, 2151677952ULL, 49152ULL, 7936ULL, 32ULL, 64ULL, 16ULL},
  {1157442765409222656ULL, 1108169195520ULL, 550831652864ULL, 57344ULL, 3840ULL, 16ULL, 32ULL, 8ULL},
  {578721382704611328ULL, 4328783872ULL, 141012904181760ULL, 61440ULL, 1792ULL, 8ULL, 16ULL, 4ULL},
  {289360691352305664ULL, 16908288ULL, 36099303471054848ULL, 63488ULL, 768ULL, 4ULL, 8ULL, 2ULL},
  {144680345676152832ULL, 65536ULL, 9241421688590303232ULL, 64512ULL, 256ULL, 2ULL, 4ULL, 1ULL},
  {72340172838076416ULL, 0ULL, 4620710844295151616ULL, 65024ULL, 0ULL, 1ULL, 2ULL, 0ULL},
  {9259542123273814016ULL, 72624976668147712ULL, 0ULL, 0ULL, 127ULL, 0ULL, 0ULL, 0ULL},
  {4629771061636907008ULL, 283691315109888ULL, 32768ULL, 128ULL, 63ULL, 0ULL, 0ULL, 0ULL},
  {2314885530818453504ULL, 1108169199616ULL, 8404992ULL, 192ULL, 31ULL, 0ULL, 0ULL, 0ULL},
  {1157442765409226752ULL, 4328785920ULL, 2151686144ULL, 224ULL, 15ULL, 0ULL, 0ULL, 0ULL},
  {578721382704613376ULL, 16909312ULL, 550831656960ULL, 240ULL, 7ULL, 0ULL, 0ULL, 0ULL},
  {289360691352306688ULL, 66048ULL, 141012904183808ULL, 248ULL, 3ULL, 0ULL, 0ULL, 0ULL},
  {144680345676153344ULL, 256ULL, 36099303471055872ULL, 252ULL, 1ULL, 0ULL, 0ULL, 0ULL},
  {72340172838076672ULL, 0ULL, 9241421688590303744ULL, 254ULL, 0ULL, 0ULL, 0ULL, 0ULL}
};
const int ms1bTable[256] = {0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7};

int bitPop(U64& number) { // TODO: Check for zero?
  #if defined(__GNUC__) || defined(__GNUG__)
    int returner = 63 - __builtin_ctzll(number);
  #else
    U64 LS1B = number & -number;
    int returner = 0;
    while(LS1B) {
      returner++;
      LS1B >>= 1;
    }
    returner = 64-returner;
  #endif
    number = number & (number-1);
    return returner;
}

int bitScan(U64 board) {
  U64 LS1B = board & -board;
  int returner = 0;
  while(LS1B) {
    returner++;
    LS1B >>= 1;
  }
  return 64-returner;
}

int bitScanReverse(U64 board)
{
  #if defined(__GNUC__) || defined(__GNUG__)
    int returner = __builtin_clzll(board);
    return returner;
  #else
   int returner = 0;
   if (board > 0xFFFFFFFF) {
      board >>= 32;
      returner = 32;
   }
   if (board > 0xFFFF) {
      board >>= 16;
      returner += 16;
   }
   if (board > 0xFF) {
      board >>= 8;
      returner += 8;
   }
   return 63 - (returner + ms1bTable[board]);
  #endif
}

linkedMoveList generateMovesFromBitboard(const U64 toBoard) {
  U64 board = toBoard;
  linkedMoveList moves;
  while(board) {
    int i = bitPop(board);
    moves.create(0, i);
  }
  return moves;
}

void constantShiftGenerator(U64 board, int shift, arrayMoveList& possibleMoves) {
  while(board != 0) {
    int i = bitPop(board);
    possibleMoves.create(i + shift, i);
  }
}

void constantShiftGenerator(U64 board, int shift, int special, arrayMoveList& possibleMoves) {
  while(board != 0) {
    int i = bitPop(board);
    possibleMoves.create(i + shift, i, special);
  }
}

bool pieceAtSquare(const U64 board, const int square) {
  if( board & 1ULL << (63ULL - (U64)square) ) {
    return true;
  } else {
    return false;
  }
}

void possibleMovesWPawns(move lastMove, const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  // Move forward one
  U64 fw1 = ( (game.WP & ~Rank_7) << 8 ) & ~(info.BlackPieces | info.WhitePieces);
  constantShiftGenerator(fw1, 8, possibleMoves);

  // Move forward two
  U64 fw2 = ((game.WP & Rank_2) << 16) & ~(info.BlackPieces | info.WhitePieces) & ~((info.BlackPieces | info.WhitePieces) << 8);
  constantShiftGenerator(fw2, 16, possibleMoves);

  // Attack
  U64 atL = ((game.WP & ~File_A & ~Rank_7) << 9) & info.BlackPieces;
  constantShiftGenerator(atL, 9, possibleMoves);

  U64 atR = ((game.WP & ~File_H & ~Rank_7) << 7) & info.BlackPieces;
  constantShiftGenerator(atR, 7, possibleMoves);

  // Promotion
  U64 pr = ((game.WP & Rank_7) << 8) & ~(info.BlackPieces | info.WhitePieces);
  while(pr != 0) {
    int i = bitPop(pr);
    for(byte j = 2; j <= 5; j++) {
      possibleMoves.create(i + 8, i, j);
    }
  }

  U64 prL = ((game.WP & Rank_7) << 9) & ~(info.WhitePieces) & info.BlackPieces;
  while(prL != 0) {
    int i = bitPop(prL);
    for(byte j = 2; j <= 5; j++) {
      possibleMoves.create(i + 9, i, j);
    }
  }

  U64 prR = ((game.WP & Rank_7) << 7) & ~(info.WhitePieces) & info.BlackPieces;
  while(prR != 0) {
    int i = bitPop(prR);
    for(byte j = 2; j <= 5; j++) {
      possibleMoves.create(i + 7, i, j);
    }
  }

  // En passant
	if(!(lastMove.start == 0 && lastMove.end == 0)) { // Was there a last move?
		if(24 <= lastMove.end && lastMove.end <= 31) { // Did it end in our range?
			if(pieceAtSquare(game.BP, lastMove.end) && 8 <= lastMove.start && lastMove.start <= 15) { // Was it an enemy pawn? Did this pawn move two?
				if(lastMove.end >= 25) // Was it in bounds on the left?
					if(pieceAtSquare(game.WP, lastMove.end - 1)) // Is there a friendly piece on the left side?
						possibleMoves.create(lastMove.end - 1, lastMove.end - 8, 10);
				if(lastMove.end <= 30) // Was it in bounds on the right?
					if(pieceAtSquare(game.WP, lastMove.end + 1)) // Is there a friendly piece on the right side?
						possibleMoves.create(lastMove.end + 1, lastMove.end - 8, 10);
			}
		}
	}
}

void possibleMovesBPawns(move lastMove, const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  // Move forward one
  U64 fw1 = ( (game.BP & ~Rank_2) >> 8 ) & ~(info.BlackPieces | info.WhitePieces);
  constantShiftGenerator(fw1, -8, possibleMoves);

  // Move forward two
  U64 fw2 = ((game.BP & Rank_7) >> 16) & ~(info.BlackPieces | info.WhitePieces) & ~((info.BlackPieces | info.WhitePieces) >> 8);
  constantShiftGenerator(fw2, -16, possibleMoves);

  // Attack
  U64 atL = ((game.BP & ~File_H & ~Rank_2) >> 9) & info.WhitePieces;
  constantShiftGenerator(atL, -9, possibleMoves);

  U64 atR = ((game.BP & ~File_A & ~Rank_2) >> 7) & info.WhitePieces;
  constantShiftGenerator(atR, -7, possibleMoves);

  // Promotion
  U64 pr = ((game.BP & Rank_2) >> 8) & ~(info.BlackPieces | info.WhitePieces);
  while(pr != 0) {
    int i = bitPop(pr);
    for(byte j = 6; j <= 9; j++) {
      possibleMoves.create(i - 8, i, j);
    }
  }

  U64 prL = ((game.BP & Rank_2) >> 7) & ~(info.BlackPieces) & info.WhitePieces;
  while(prL != 0) {
    int i = bitPop(prL);
    for(byte j = 6; j <= 9; j++) {
      possibleMoves.create(i - 7, i, j);
    }
  }

  U64 prR = ((game.BP & Rank_2) >> 9) & ~(info.BlackPieces) & info.WhitePieces;
  while(prR != 0) {
    int i = bitPop(prR);
    for(byte j = 6; j <= 9; j++) {
      possibleMoves.create(i - 9, i, j);
    }
  }

  // En passant
	if(!(lastMove.start == 0 && lastMove.end == 0)) { // Was there a last move?
		if(32 <= lastMove.end && lastMove.end <= 39) { // Did it end in our range?
			if(pieceAtSquare(game.WP, lastMove.end)  && 48 <= lastMove.start && lastMove.start <= 55) { // Was it an enemy pawn? Did this pawn move two?
				if(lastMove.end >= 33) // Was it in bounds on the left?
					if(pieceAtSquare(game.BP, lastMove.end - 1)) // Is there a friendly piece on the left side?
						possibleMoves.create(lastMove.end - 1, lastMove.end + 8, 11);
				if(lastMove.end <= 38) // Was it in bounds on the right?
					if(pieceAtSquare(game.BP, lastMove.end + 1)) // Is there a friendly piece on the right side?
						possibleMoves.create(lastMove.end + 1, lastMove.end + 8, 11);
			}
		}
	}
}

void possibleMovesKnights(const U64 kBoard, const U64 FriendlyPieces, arrayMoveList& possibleMoves) {
  //Left two up one
  U64 l2u1 = ((kBoard & ~(File_A | File_B | Rank_8)) << 10) & ~FriendlyPieces; // Posible trival optimization by precomupting const bitboard combinations
  constantShiftGenerator(l2u1, 10, possibleMoves);

  //Right two up one
  U64 r2u1 = ((kBoard & ~(File_G | File_H | Rank_8)) << 6) & ~FriendlyPieces;
  constantShiftGenerator(r2u1, 6, possibleMoves);

  //Left one up two
  U64 l1u2 = ((kBoard & ~(File_A | Rank_7 | Rank_8)) << 17) & ~FriendlyPieces;
  constantShiftGenerator(l1u2, 17, possibleMoves);

  //Right one up two
  U64 r1u2 = ((kBoard & ~(File_H | Rank_7 | Rank_8)) << 15) & ~FriendlyPieces;
  constantShiftGenerator(r1u2, 15, possibleMoves);

  //Left two down one
  U64 l2d1 = ((kBoard & ~(File_A | File_B | Rank_1)) >> 6) & ~FriendlyPieces;
  constantShiftGenerator(l2d1, -6, possibleMoves);

  //Right two down one
  U64 r2d1 = ((kBoard & ~(File_G | File_H | Rank_1)) >> 10) & ~FriendlyPieces;
  constantShiftGenerator(r2d1, -10, possibleMoves);

  //Left one down two
  U64 l1d2 = ((kBoard & ~(File_A | Rank_1 | Rank_2)) >> 15) & ~FriendlyPieces;
  constantShiftGenerator(l1d2, -15, possibleMoves);

  //Right one down two
  U64 r1d2 = ((kBoard & ~(File_H | Rank_1 | Rank_2)) >> 17) & ~FriendlyPieces;
  constantShiftGenerator(r1d2, -17, possibleMoves);
}

void possibleMovesWKnights(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesKnights(game.WN, info.WhitePieces, possibleMoves);
}

void possibleMovesBKnights(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesKnights(game.BN, info.BlackPieces, possibleMoves);
}

void possibleMovesKings(const U64 kBoard, const U64 FriendlyPieces, arrayMoveList& possibleMoves) {
  U64 u = ((kBoard & ~(Rank_8)) << 8) & ~FriendlyPieces;
  constantShiftGenerator(u, 8, possibleMoves);

  U64 ur = ((kBoard & ~(File_H | Rank_8)) << 7) & ~FriendlyPieces;
  constantShiftGenerator(ur, 7, possibleMoves);

  U64 ul = ((kBoard & ~(File_A | Rank_8)) << 9) & ~FriendlyPieces;
  constantShiftGenerator(ul, 9, possibleMoves);

  U64 l = ((kBoard & ~(File_A)) << 1) & ~FriendlyPieces;
  constantShiftGenerator(l, 1, possibleMoves);

  U64 r = ((kBoard & ~(File_H)) >> 1) & ~FriendlyPieces;
  constantShiftGenerator(r, -1, possibleMoves);

  U64 d = ((kBoard & ~(Rank_1)) >> 8) & ~FriendlyPieces;
  constantShiftGenerator(d, -8, possibleMoves);

  U64 dr = ((kBoard & ~(File_H | Rank_1)) >> 9) & ~FriendlyPieces;
  constantShiftGenerator(dr, -9, possibleMoves);

  U64 dl = ((kBoard & ~(File_A | Rank_1)) >> 7) & ~FriendlyPieces;
  constantShiftGenerator(dl, -7, possibleMoves);
}

void possibleMovesWKings(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesKings(game.WK, info.WhitePieces, possibleMoves);
}

void possibleMovesBKings(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesKings(game.BK, info.BlackPieces, possibleMoves);
}

// TODO: Optimization possible. Reduction from ints? Mask consolidation?
linkedMoveList rayAttack(const int origin, const int modifier, const U64 NoNoZone, const U64 enemies, const U64 friendlies) {
  linkedMoveList possibleMoves;
  int cursor = origin;

  while (true) {
    if(pieceAtSquare(NoNoZone, cursor)) {
      break;
    }

    cursor += modifier;

    if(pieceAtSquare(friendlies, cursor)) {
      break;
    }
    if(pieceAtSquare(enemies, cursor)) {
      possibleMoves.create(origin, cursor);
      break;
    }
    possibleMoves.create(origin, cursor);
  }

  return possibleMoves;
}

U64 getPositiveRayAttacks(U64 occupied, U64 friendly, int direction, int square) {
  U64 attacks = RayTable[square][direction];
  U64 blocker = attacks & occupied;
  if(blocker) {
    square = bitScanReverse(blocker); // bitScans are reversed from normal naming because our square mapping is different
    attacks ^= RayTable[square][direction];
  }
  attacks &= ~friendly;
  return attacks;
}

U64 getNegativeRayAttacks(U64 occupied, U64 friendly, int direction, int square) {
  U64 attacks = RayTable[square][direction];
  U64 blocker = attacks & occupied;
  if(blocker) {
    square = bitScan(blocker);
    attacks ^= RayTable[square][direction];
  }
  attacks &= ~friendly;
  return attacks;
}

void possibleMovesRooks(const U64 rBoard, const U64 FriendlyPieces, const U64 EnemyPieces, castleBools castleInfo, bool black, arrayMoveList& possibleMoves) {
  U64 pieces = rBoard;

  const U64 occupied = FriendlyPieces | EnemyPieces;

  while(pieces) {
    int square = bitPop(pieces);
    U64 attacks = 0ULL;

    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 0, square);
    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 3, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 4, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 5, square);

    while(attacks) {
      int j = bitPop(attacks);
      possibleMoves.create(square, j);
    }
  }

	if(black) {
		if(castleInfo.blackACan) {
			if(!(8070450532247928832ULL & (FriendlyPieces | EnemyPieces) )) // Checks for pieces between the rook and king
				possibleMoves.create(4, 2, 1);
		}
		if(castleInfo.blackHCan) {
			if(!(432345564227567616ULL & (FriendlyPieces | EnemyPieces) ))
				possibleMoves.create(4, 6, 1);
		}
	} else {
		if(castleInfo.whiteACan) {
			if(!(112ULL & (FriendlyPieces | EnemyPieces) ))
				possibleMoves.create(60, 58, 1);
		}
		if(castleInfo.whiteHCan) {
			if(!(6ULL & (FriendlyPieces | EnemyPieces) ))
				possibleMoves.create(60, 62, 1);
		}
	}
}

void possibleMovesWRooks(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesRooks(game.WR, info.WhitePieces, info.BlackPieces, game.castleInfo, false, possibleMoves);
}

void possibleMovesBRooks(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesRooks(game.BR, info.BlackPieces, info.WhitePieces, game.castleInfo, true, possibleMoves);
}

void possibleMovesBishops(const U64 rBoard, const U64 FriendlyPieces, const U64 EnemyPieces, arrayMoveList& possibleMoves) {
  U64 pieces = rBoard;

  const U64 occupied = FriendlyPieces | EnemyPieces;

  while(pieces) {
    int square = bitPop(pieces);
    U64 attacks = 0ULL;

    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 1, square);
    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 2, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 6, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 7, square);

    while(attacks) {
      int j = bitPop(attacks);
      possibleMoves.create(square, j);
    }
  }
}

void possibleMovesWBishops(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesBishops(game.WB, info.WhitePieces, info.BlackPieces, possibleMoves);
}

void possibleMovesBBishops(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesBishops(game.BB, info.BlackPieces, info.WhitePieces, possibleMoves);
}

void possibleMovesQueens(const U64 qBoard, const U64 FriendlyPieces, const U64 EnemyPieces, arrayMoveList& possibleMoves) {
  U64 pieces = qBoard;

  const U64 occupied = FriendlyPieces | EnemyPieces;

  while(pieces) {
    int square = bitPop(pieces);
    U64 attacks = 0ULL;

    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 0, square);
    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 1, square);
    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 2, square);
    attacks |= getNegativeRayAttacks(occupied, FriendlyPieces, 3, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 4, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 5, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 6, square);
    attacks |= getPositiveRayAttacks(occupied, FriendlyPieces, 7, square);


    while(attacks) {
      int j = bitPop(attacks);
      possibleMoves.create(square, j);
    }
  }
}

void possibleMovesWQueens(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesQueens(game.WQ, info.WhitePieces, info.BlackPieces, possibleMoves);
}

void possibleMovesBQueens(const bitboards game, const extraBitboardsInfo info, arrayMoveList& possibleMoves) {
  possibleMovesQueens(game.BQ, info.BlackPieces, info.WhitePieces, possibleMoves);
}

// List all moves the player can make for the given game
// Only guaranteed for standard chess
arrayMoveList possibleMovesW(move lastMove, const bitboards game) {
  arrayMoveList possibleMoves;

  extraBitboardsInfo info;
  info.updateFromBitboards(game);

  possibleMovesWPawns(lastMove, game, info, possibleMoves);
  possibleMovesWKnights(game, info, possibleMoves);
  possibleMovesWKings(game, info, possibleMoves);
  possibleMovesWRooks(game, info, possibleMoves);
  possibleMovesWBishops(game, info, possibleMoves);
  possibleMovesWQueens(game, info, possibleMoves);

  return possibleMoves;
}

arrayMoveList possibleMovesB(move lastMove, const bitboards game) {
  arrayMoveList possibleMoves;

  extraBitboardsInfo info;
  info.updateFromBitboards(game);

  possibleMovesBPawns(lastMove, game, info, possibleMoves);
  possibleMovesBKnights(game, info, possibleMoves);
  possibleMovesBKings(game, info, possibleMoves);
  possibleMovesBRooks(game, info, possibleMoves);
  possibleMovesBBishops(game, info, possibleMoves);
  possibleMovesBQueens(game, info, possibleMoves);

  return possibleMoves;
}

void setAllToZero(bitboards* game, U64 endMask) {
  U64 mask = ~endMask;
  game->WP &= mask;
  game->WN &= mask;
  game->WB &= mask;
  game->WR &= mask;
  game->WQ &= mask;
  game->WK &= mask;
  game->BP &= mask;
  game->BN &= mask;
  game->BB &= mask;
  game->BR &= mask;
  game->BQ &= mask;
  game->BK &= mask;
}

bool checkBoardMove(U64& board, U64 startMask, U64 endMask) { // TODO: possible optimization by making returnless version
  if( board & startMask ) {
    board &= ~startMask;
    board |= endMask;
		return true;
  }
	return false;
}

// Note: lots of room for optimization
void applyMove(bitboards* game, move change) {
  U64 startMask = 1ULL << (63ULL - (U64)change.start);
  U64 endMask = 1ULL << (63ULL - (U64)change.end);

  if(change.end == 0) {
    game->castleInfo.blackACan = false;
  } else if(change.end == 7) {
    game->castleInfo.blackHCan = false;
  } else if(change.end == 56) {
    game->castleInfo.whiteACan = false;
  } else if(change.end == 63) {
    game->castleInfo.whiteHCan = false;
  }

  setAllToZero(game, endMask);

  checkBoardMove(game->WP, startMask, endMask);
  checkBoardMove(game->WN, startMask, endMask);
  checkBoardMove(game->WB, startMask, endMask);
  checkBoardMove(game->WR, startMask, endMask);
  checkBoardMove(game->WQ, startMask, endMask);
  if(checkBoardMove(game->WK, startMask, endMask)) { // TODO: Posible optimization by checking position number instead
		game->castleInfo.whiteACan = false;
		game->castleInfo.whiteHCan = false;
	}
  checkBoardMove(game->BP, startMask, endMask);
  checkBoardMove(game->BN, startMask, endMask);
  checkBoardMove(game->BB, startMask, endMask);
  checkBoardMove(game->BR, startMask, endMask);
  checkBoardMove(game->BQ, startMask, endMask);
  if(checkBoardMove(game->BK, startMask, endMask)) {
		game->castleInfo.blackACan = false;
		game->castleInfo.blackHCan = false;
	}

	if(change.start == 63) {
		game->castleInfo.whiteHCan = false;
	} else if(change.start == 56) {
		game->castleInfo.whiteACan = false;
	} else if(change.start == 0) {
		game->castleInfo.blackACan = false;
	} else if(change.start == 7)
		game->castleInfo.blackHCan = false;

  switch(change.special) { // Check moves.h for the list of special moves. TODO: use an enum instead of a manual code
		case 1:
			if(change.start < 8) {
        //game->castleInfo.blackACan = false;
        //game->castleInfo.blackHCan = false;
				if(change.end == 2) {
          setAllToZero(game, 1ULL << (63ULL - (U64)0));
					game->BR |= 1ULL << (63ULL - (U64)3);
				} else {
          setAllToZero(game, 1ULL << (63ULL - (U64)7));
					game->BR |= 1ULL << (63ULL - (U64)5);
				}
			} else {
        //game->castleInfo.whiteACan = false;
        //game->castleInfo.whiteHCan = false;
				if(change.end == 58) {
          setAllToZero(game, 1ULL << (63ULL - (U64)56));
					game->WR |= 1ULL << (63ULL - (U64)59);
				} else {
          setAllToZero(game, 1ULL << (63ULL - (U64)63));
					game->WR |= 1ULL << (63ULL - (U64)61);
				}
			}
			break;
    case 2:
      setAllToZero(game, endMask);
      game->WQ |= endMask;
      break;
    case 3:
      setAllToZero(game, endMask);
      game->WR |= endMask;
      break;
    case 4:
      setAllToZero(game, endMask);
      game->WN |= endMask;
      break;
    case 5:
      setAllToZero(game, endMask);
      game->WB |= endMask;
      break;
    case 6:
      setAllToZero(game, endMask);
      game->BQ |= endMask;
      break;
    case 7:
      setAllToZero(game, endMask);
      game->BR |= endMask;
      break;
    case 8:
      setAllToZero(game, endMask);
      game->BN |= endMask;
      break;
    case 9:
      setAllToZero(game, endMask);
      game->BB |= endMask;
      break;
    case 10:
      setAllToZero(game, endMask >> 8);
      break;
    case 11:
      setAllToZero(game, endMask << 8);
      break;
    default:
      break;
  }
}

bool attackOnKing(move change, const bitboards game) {
  U64 endMask = 1ULL << (63ULL - (U64)change.end);

  if(game.WK & endMask || game.BK & endMask) {
    return true;
  }
  return false;
}

bool missingKing(const bitboards game) {
  if(game.WK && game.BK) {
    return false;
  }
  return true;
}

bool inCheck(bool checkBlacks, const bitboards game) {
  move nullMove; // Won't be needed since kings cannot be put into check via en Passant
  arrayMoveList moves;
  if(checkBlacks) {
    moves = possibleMovesW(nullMove, game);
  } else {
    moves = possibleMovesB(nullMove, game);
  }

  moves.resetIterator();
  while(moves.next()) {
    if(attackOnKing(moves.getMove(), game))
    {
      return true;
    }
  }
  return false;
}
