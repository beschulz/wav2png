#import "WaveformDropImageView.h"
#import "AppDelegate.h"
#include <sndfile.hh>
#include <png++/png.hpp>
#include "wav2png.hpp"
#include <sstream>

static int running_jobs = 0;

@implementation WaveformDropImageView

NSString *kPrivateDragUTI = @"de.betabugs.cocoadraganddrop";

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (id)initWithCoder:(NSCoder *)coder
{
    /*------------------------------------------------------
     Init method called for Interface Builder objects
     --------------------------------------------------------*/
    self=[super initWithCoder:coder];
    if ( self ) {
        //register for all the image types we can display
        //[self registerForDraggedTypes:[NSImage imagePasteboardTypes]];
        //[self registerForDraggedTypes:[NSSound soundUnfilteredTypes]];
    }
    return self;
}

#pragma mark - Destination Operations

- (bool)isSupportedFormat:(NSString*)file_name
{
    /*const char* file_name_ = [file_name cs];
    
    SndfileHandle handle();*/
    
    SndfileHandle handle( [file_name UTF8String] );
    if ( !handle.error() && handle.frames() > 0 )
        return true;
    
    return false;
}

- (NSString*) getDraggedPath:(id <NSDraggingInfo>)sender
{
    NSPasteboard *pboard = [sender draggingPasteboard];
    
    if ( [[pboard types] containsObject:NSURLPboardType] )
    {
        NSURL *fileURL = [NSURL URLFromPasteboard:pboard];
        return [fileURL path];
    }
    
    return nil;
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
    NSString* path = [self getDraggedPath: sender];
    if ( running_jobs == 0 && path && [self isSupportedFormat:path] )
    {
        NSLog(@"%@\n", path);
        return NSDragOperationCopy; 
    }
    return NSDragOperationNone;
}

- (void)draggingExited:(id <NSDraggingInfo>)sender
{
    /*------------------------------------------------------
     method called whenever a drag exits our drop zone
     --------------------------------------------------------*/
    //remove highlight of the drop zone
    highlight=NO;
    [self setNeedsDisplay: YES];
}

-(void)drawRect:(NSRect)rect
{
    /*------------------------------------------------------
     draw method is overridden to do drop highlighing
     --------------------------------------------------------*/
    //do the usual draw operation to display the image
    [super drawRect:rect];
    
    if ( highlight ) {
        //highlight by overlaying a gray border
        [[NSColor grayColor] set];
        [NSBezierPath setDefaultLineWidth: 5];
        [NSBezierPath strokeRect: rect];
    }
}

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender
{
    /*------------------------------------------------------
     method to determine if we can accept the drop
     --------------------------------------------------------*/
    //finished with the drag so remove any highlighting
    highlight=NO;
    [self setNeedsDisplay: YES];
    
    return [self isSupportedFormat:[self getDraggedPath:sender]];
} 

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
    /*------------------------------------------------------
     method that should handle the drop data
     --------------------------------------------------------*/
    if ( [sender draggingSource] != self )
    {
        NSString* path = [self getDraggedPath:sender];
        
        dispatch_async(dispatch_get_global_queue(0,0), 
        ^{
            running_jobs++;
            
            AppDelegate* app_delegate = [NSApp delegate];
            NSColor* fg = [app_delegate.foreground color];
            NSColor* bg = [app_delegate.background color];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                [[self window] setTitle: @"Converting … please wait!"];
            });
                        
            SndfileHandle wav([path UTF8String]);
            
            int width  = std::max(1, [app_delegate.width_box  intValue]);
            int height = std::max(1, [app_delegate.height_box intValue]);
            png::image< png::rgba_pixel > image(
                width,
                height
            );

            png::rgba_pixel foreground_color(
                255*[fg redComponent],
                255*[fg greenComponent],
                255*[fg blueComponent],
                255*[fg alphaComponent]
            );
            
            png::rgba_pixel background_color(
                255*[bg redComponent],
                255*[bg greenComponent],
                255*[bg blueComponent],
                255*[bg alphaComponent]
            );
            
            //png::rgba_pixel bg_color(0xef, 0xef, 0xef, 255);
            compute_waveform(
                             wav,
                             image,
                             background_color,
                             foreground_color,
                             app_delegate.log_scale.state,
                             -40,
                             0,
                             app_delegate.line_only.state, //line only
                             ^(int p){
                                 dispatch_async(dispatch_get_main_queue(), ^{
                                     [[self window] setTitle: [NSString stringWithFormat:@"converting: %d%%", p]];
                                 });
                                 
                                 return true;
                             }
            );
            
            std::clog << image.get_height() << std::endl;
            
            std::stringstream stream;
            image.write_stream(stream);
            const std::string& c_data = stream.str();
            
            NSData* data = [[NSData alloc] initWithBytes:c_data.c_str() length: c_data.size()];
            NSImage* new_image = [[NSImage alloc] initWithData: data];
            
            NSSize size;
            size.width = size.height = 1;
            [new_image setSize:size];
            
            dispatch_async(dispatch_get_main_queue(), ^{
                [self setImage:new_image];
            });

            // write image to disk
            if ( app_delegate.autosave.state  )
            {
                NSArray *paths = NSSearchPathForDirectoriesInDomains(NSPicturesDirectory, NSUserDomainMask, YES);
                
                //this gets the pictures directory in sandbox container not the actual user Pictures directory
                NSString *userPicturesPath = [[paths objectAtIndex:0] stringByResolvingSymlinksInPath];
                
                auto baseName = [[NSURL URLWithString:path] lastPathComponent];

                auto subFolder = [[NSURL fileURLWithPath:userPicturesPath] URLByAppendingPathComponent:@"wav2png"];
                [[NSFileManager defaultManager] createDirectoryAtURL:subFolder withIntermediateDirectories:YES attributes:nil error:nil];

                auto fullPath = [subFolder URLByAppendingPathComponent:baseName].path;

                image.write(std::string([fullPath UTF8String]) + ".png");

                //image.write(std::string([path UTF8String]) + ".png");
            }

            dispatch_async(dispatch_get_main_queue(), ^{
                [[self window] setTitle: path!=NULL ? path : @"(no name)"];
            });

            running_jobs--;
        });
    }
    
    return YES;
}

- (NSRect)windowWillUseStandardFrame:(NSWindow *)window defaultFrame:(NSRect)newFrame;
{
    /*------------------------------------------------------
     delegate operation to set the standard window frame
     --------------------------------------------------------*/
    //get window frame size
    NSRect ContentRect=self.window.frame;
    
    //set it to the image frame size
    ContentRect.size=[[self image] size];
    
    return [NSWindow frameRectForContentRect:ContentRect styleMask: [window styleMask]];
};

#pragma mark - Source Operations

- (void)mouseDown:(NSEvent*)event
{
    /*------------------------------------------------------
     catch mouse down events in order to start drag
     --------------------------------------------------------*/
    
    /* Dragging operation occur within the context of a special pasteboard (NSDragPboard).
     * All items written or read from a pasteboard must conform to NSPasteboardWriting or 
     * NSPasteboardReading respectively.  NSPasteboardItem implements both these protocols
     * and is as a container for any object that can be serialized to NSData. */
    
    NSPasteboardItem *pbItem = [NSPasteboardItem new];
    /* Our pasteboard item will support public.tiff, public.pdf, and our custom UTI (see comment in -draggingEntered)
     * representations of our data (the image).  Rather than compute both of these representations now, promise that 
     * we will provide either of these representations when asked.  When a receiver wants our data in one of the above 
     * representations, we'll get a call to  the NSPasteboardItemDataProvider protocol method –pasteboard:item:provideDataForType:. */
    [pbItem setDataProvider:self forTypes:[NSArray arrayWithObjects:NSPasteboardTypeTIFF, NSPasteboardTypePDF, kPrivateDragUTI, nil]];
    
    //create a new NSDraggingItem with our pasteboard item.
    NSDraggingItem *dragItem = [[NSDraggingItem alloc] initWithPasteboardWriter:pbItem];
    [pbItem release];
    
    /* The coordinates of the dragging frame are relative to our view.  Setting them to our view's bounds will cause the drag image
     * to be the same size as our view.  Alternatively, you can set the draggingFrame to an NSRect that is the size of the image in
     * the view but this can cause the dragged image to not line up with the mouse if the actual image is smaller than the size of the
     * our view. */
    NSRect draggingRect = self.bounds;
    
    /* While our dragging item is represented by an image, this image can be made up of multiple images which
     * are automatically composited together in painting order.  However, since we are only dragging a single
     * item composed of a single image, we can use the convince method below. For a more complex example
     * please see the MultiPhotoFrame sample. */
    [dragItem setDraggingFrame:draggingRect contents:[self image]];
    
    //create a dragging session with our drag item and ourself as the source.
    NSDraggingSession *draggingSession = [self beginDraggingSessionWithItems:[NSArray arrayWithObject:[dragItem autorelease]] event:event source:self];
    //causes the dragging item to slide back to the source if the drag fails.
    draggingSession.animatesToStartingPositionsOnCancelOrFail = YES;
    
    draggingSession.draggingFormation = NSDraggingFormationNone;
}

- (NSDragOperation)draggingSession:(NSDraggingSession *)session sourceOperationMaskForDraggingContext:(NSDraggingContext)context
{
    /*------------------------------------------------------
     NSDraggingSource protocol method.  Returns the types of operations allowed in a certain context.
     --------------------------------------------------------*/
    switch (context) {
        case NSDraggingContextOutsideApplication:
            return NSDragOperationCopy;
            
            //by using this fall through pattern, we will remain compatible if the contexts get more precise in the future.
        case NSDraggingContextWithinApplication:
        default:
            return NSDragOperationCopy;
            break;
    }
}

- (BOOL)acceptsFirstMouse:(NSEvent *)event 
{
    /*------------------------------------------------------
     accept activation click as click in window
     --------------------------------------------------------*/
    //so source doesn't have to be the active window
    return YES;
}

- (void)pasteboard:(NSPasteboard *)sender item:(NSPasteboardItem *)item provideDataForType:(NSString *)type
{
    /*------------------------------------------------------
     method called by pasteboard to support promised 
     drag types.
     --------------------------------------------------------*/
    //sender has accepted the drag and now we need to send the data for the type we promised
    if ( [type compare: NSPasteboardTypeTIFF] == NSOrderedSame ) {
        
        //set data for TIFF type on the pasteboard as requested
        [sender setData:[[self image] TIFFRepresentation] forType:NSPasteboardTypeTIFF];
        
    } else if ( [type compare: NSPasteboardTypePDF] == NSOrderedSame ) {
        
        //set data for PDF type on the pasteboard as requested
        [sender setData:[self dataWithPDFInsideRect:[self bounds]] forType:NSPasteboardTypePDF];
    }
    
}

@end
