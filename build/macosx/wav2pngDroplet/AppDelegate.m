#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window = _window;

@synthesize foreground;
@synthesize background;
@synthesize width_box;
@synthesize height_box;
@synthesize waveform;
@synthesize autosave;
@synthesize log_scale;
@synthesize line_only;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [NSColor setIgnoresAlpha:NO];
    [waveform setImageScaling:NSScaleToFit];
    
    [[NSNotificationCenter defaultCenter]
     addObserver:self
     selector:@selector(windowClosing:)
     name:NSWindowWillCloseNotification
     object:_window];

    [[NSUserDefaults standardUserDefaults] registerDefaults:[NSDictionary dictionaryWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"Defaults" ofType:@"plist"]]];
}

- (void)windowClosing:(NSNotification*)aNotification
{
    NSLog(@"foreground = %@", [NSArchiver archivedDataWithRootObject:foreground.color]);
    NSLog(@"background = %@", [NSArchiver archivedDataWithRootObject:background.color]);

    [NSApp terminate: nil];
}


@end
