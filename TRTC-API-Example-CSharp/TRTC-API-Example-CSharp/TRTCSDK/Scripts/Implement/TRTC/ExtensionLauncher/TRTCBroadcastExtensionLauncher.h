// Copyright (c) 2023 Tencent. All rights reserved.
// Author: felixyyan

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface TRTCBroadcastExtensionLauncher : NSObject
+ (instancetype)sharedInstance;
+ (void)launch;
@end

NS_ASSUME_NONNULL_END