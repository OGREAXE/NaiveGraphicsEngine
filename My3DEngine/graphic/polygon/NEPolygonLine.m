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

+ (NEPolygonLine *)lineWithStartArray:(NSArray<NSNumber*>*)start endArray:(NSArray<NSNumber*>*)end{
    NEVector3 v0 = {
        start[0].floatValue,
        start[1].floatValue,
        start[2].floatValue
    };
    
    NEVector3 v1 = {
        end[0].floatValue,
        end[1].floatValue,
        end[2].floatValue
    };
    
    return [NEPolygonLine lineWithStart:v0 end:v1];
}

@end
