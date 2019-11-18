//
//  MEPolygonLine.m
//  My3DEngine
//
//  Created by Liang,Zhiyuan(MTD) on 2019/10/24.
//  Copyright © 2019 Liang,Zhiyuan(MTD). All rights reserved.
//

#import "NEPolygonLine.h"

@implementation NEPolygonLine

+ (NEPolygonLine *)lineWithStart:(NEVector3)start end:(NEVector3)end{
    NEPolygonLine * line = [[NEPolygonLine alloc] init];
    line.startPosition = start;
    line.endPosition = end;
    return line;
}

@end
