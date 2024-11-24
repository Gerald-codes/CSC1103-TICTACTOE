#import <AVFoundation/AVFoundation.h>
#import <Foundation/Foundation.h>

// Declare a static player to keep it alive during playback
static AVAudioPlayer *player;

void play_song(const char *filename) {
    @autoreleasepool {
        NSString *path = [NSString stringWithUTF8String:filename];
        NSURL *fileURL = [NSURL fileURLWithPath:path];
        NSError *error = nil;

        player = [[AVAudioPlayer alloc] initWithContentsOfURL:fileURL error:&error];
        if (!player) {
            NSLog(@"Error initializing AVAudioPlayer: %@", [error localizedDescription]);
            return;
        }

        [player prepareToPlay];
        [player play];  // Start playing asynchronously

        // No need to block with `NSRunLoop`; just return
        NSLog(@"Playing song: %@", path);
    }
}