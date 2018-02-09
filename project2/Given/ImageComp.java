import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.IOException;

import java.awt.image.BufferedImage;
import java.awt.Color;
import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.Icon;
import javax.swing.ImageIcon;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

/** Display and compare two images in pgm format. */
public class ImageComp implements ActionListener {
  /** Image for the first command-line argument. */
  Icon left;
  
  /** Image for the second command-line argument. */
  Icon right;
  
  /** Image showing the differences between the images in pink. */
  Icon both;
  
  /** Label, for showing the various images. */
  JLabel display;

  private void usage() {
    System.err.printf( "usage: ImageComp <image1.ppm> [image2.ppm]\n" );
    System.exit( 1 );
  }

  /** Read and return the next token.  This function is flawed in that it
      doesn't put the delimiter after the token back on the stream. */
  private String getToken( InputStream stream ) throws IOException {
    // Skip whitespace at the start of this token.
    int c = stream.read();
    while ( c == -1 && Character.isWhitespace( (char) c ) )
      c = stream.read();

    if ( c == -1 ) {
      System.out.println( "Reached EOF while expecting a token\n" );
      System.exit( 1 );
    }
    
    StringBuilder str = new StringBuilder();
    while ( c != -1 && ! Character.isWhitespace( (char) c ) ) {
      str.append( (char) c );
      c = stream.read();
    }

    return str.toString();
  }

  /** Read an image from the given file and return it as an image. */
  private BufferedImage loadImage( String filename ) {
    try {
      // This function has to be a little weird.  It has to be able to read text
      // and binary data from the same stream.
      BufferedInputStream input =
        new BufferedInputStream( new FileInputStream( filename ) );

      String code = getToken( input );

      if ( ! code.equals( "P3" ) && ! code.equals( "P6" ) ) {
        System.err.printf( "File %s doesn't look like a PPM file.\n", filename );
        usage();
      }

      int width = Integer.parseInt( getToken( input ) );
      int height = Integer.parseInt( getToken( input ) );
      int cmax = Integer.parseInt( getToken( input ) );
      
      if ( cmax != 255 ) {
        System.err.printf( "File %s doesn't look like a PPM image in the right format.\n", filename );
        usage();
      }

      // Make an image and populate it with pixel values from the input file.
      BufferedImage img = new BufferedImage( width * 2, height * 2, BufferedImage.TYPE_INT_ARGB );


      if ( code.equals( "P3" ) ) {
        for ( int i = 0; i < height; i++ )
          for ( int j = 0; j < width; j++ ) {
            int r = Integer.parseInt( getToken( input ) );
            int g = Integer.parseInt( getToken( input ) );
            int b = Integer.parseInt( getToken( input ) );
            int c = new Color( r, g, b ).getRGB();
          
            // Four pixels for each input pixel.
            img.setRGB( j * 2, i * 2, c );
            img.setRGB( j * 2, i * 2 + 1, c );
            img.setRGB( j * 2 + 1, i * 2, c );
            img.setRGB( j * 2 + 1, i * 2 + 1, c );
          }
      } else {
        for ( int i = 0; i < height; i++ )
          for ( int j = 0; j < width; j++ ) {
            int r = input.read();
            int g = input.read();
            int b = input.read();

            if ( r == -1 || g == -1 || b == -1 ) {
              System.err.printf( "Reached EOF in %s while reading color values.\n", filename );
              System.exit( 1 );
            }
            
            int c = new Color( r, g, b ).getRGB();
          
            // Four pixels for each input pixel.
            img.setRGB( j * 2, i * 2, c );
            img.setRGB( j * 2, i * 2 + 1, c );
            img.setRGB( j * 2 + 1, i * 2, c );
            img.setRGB( j * 2 + 1, i * 2 + 1, c );
          }
      }


      input.close();

      return img;
    } catch ( IOException e ) {
      System.err.printf( "Error reading image from file: %s\n", filename );
      usage();
    }

    return null;
  }

  /** To respond to button presses in the UI. */
  public void actionPerformed( ActionEvent e ) {
    if ( "left".equals( e.getActionCommand() ) )
      display.setIcon( left );
    else if ( "right".equals( e.getActionCommand() ) )
      display.setIcon( right );
    else
      display.setIcon( both );
  }

  /** Process command-line arguments and create UI. */
  private void run( String[] args ) {
    if ( args.length < 1 || args.length > 2 )
      usage();

    // Read the one or two images.
    BufferedImage leftImg, rightImg = null, bothImg = null;
    leftImg = loadImage( args[ 0 ] );
    if ( args.length > 1 ) {
      rightImg = loadImage( args[ 1 ] );

      // Generate the difference image.
      if ( leftImg.getWidth() != rightImg.getWidth() ||
           leftImg.getHeight() != rightImg.getHeight() ) {
        System.err.printf( "Images aren't the same size\n" );
        System.exit( 1 );
      }

      bothImg = new BufferedImage( leftImg.getWidth(), leftImg.getHeight(), BufferedImage.TYPE_INT_ARGB );
      for ( int i = 0; i < leftImg.getHeight(); i++ )
        for ( int j = 0; j < leftImg.getWidth(); j++ ) {
          int a = leftImg.getRGB( j, i );
          int b = rightImg.getRGB( j, i );
          if ( a != b )
            bothImg.setRGB( j, i, Color.PINK.getRGB() );
          else
            bothImg.setRGB( j, i, a );
        }
    }

    // Build the UI.
    JFrame frame = new JFrame( "ImageComp" );
    frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
    frame.setLayout( new BorderLayout() );

    left = new ImageIcon( leftImg );
    display = new JLabel( left );
    frame.add( display, BorderLayout.CENTER );

    // If we have two arguments, make UI controls to switch among the
    // three images.
    if ( args.length > 1 ) {
      JPanel buttons = new JPanel();

      right = new ImageIcon( rightImg );
      both = new ImageIcon( bothImg );

      buttons.setLayout( new FlowLayout() );

      JButton btn = new JButton( args[ 0 ] );
      btn.setActionCommand( "left" );
      btn.addActionListener( this );
      buttons.add( btn );

      // If the press the right-hand button, just show the right-hand image.
      btn = new JButton( args[ 1 ] );
      btn.setActionCommand( "right" );
      btn.addActionListener( this );
      buttons.add( btn );
      
      btn = new JButton( "difference" );
      btn.addActionListener( this );
      buttons.add( btn );

      frame.add( buttons, BorderLayout.NORTH );
    }

    frame.pack();
    frame.setVisible( true );
  }
  
  public static void main( String[] args ) {
    // Make an instance of imagecomp, and run it.  That way, we can
    // have fields if we need them.
    ImageComp comp = new ImageComp();
    comp.run( args );
  }
}
