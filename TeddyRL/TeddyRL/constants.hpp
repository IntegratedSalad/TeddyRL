//
//  constants.hpp
//  TeddyRL
//
//  Created by Dev on 23/12/2021.
//  Copyright © 2021 Dev. All rights reserved.
//

#ifndef constants_h
#define constants_h

#if DEBUG && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().parent_path().parent_path().parent_path().string()
#endif

#if RELEASE && __APPLE__
#define GET_PATH_STR_WORKDIR_MACOS(x) x.parent_path().string()
#endif


const int C_SCREEN_WIDTH  = 1600;
const int C_SCREEN_HEIGHT = 1400;
const int C_TILE_TILESET_SIZE = 16;
const int C_TILE_IN_GAME_SIZE = 32;

const int C_MAP_SIZE = 40;

#endif /* constants_h */
