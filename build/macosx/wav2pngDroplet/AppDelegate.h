#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (assign) IBOutlet NSWindow *window;

@property (assign) IBOutlet NSColorWell *foreground;
@property (assign) IBOutlet NSColorWell *background;

@property (assign) IBOutlet NSTextField *width_box;
@property (assign) IBOutlet NSTextField *height_box;

@property (assign) IBOutlet NSImageView *waveform;

@property (assign) IBOutlet NSButton *autosave;
@property (assign) IBOutlet NSButton *log_scale;
@property (assign) IBOutlet NSButton *line_only;

@end
