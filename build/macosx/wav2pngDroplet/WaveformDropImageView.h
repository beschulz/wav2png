#import <Cocoa/Cocoa.h>

@interface WaveformDropImageView : NSImageView<NSDraggingSource, NSDraggingDestination, NSPasteboardItemDataProvider>
{
    BOOL highlight;
}
- (id)initWithCoder:(NSCoder *)coder;

@end
