void
preenche_dados_exif( foto_t::ptr foto )
{
#ifdef HAVE_LIBEXIF
  ExifData *ed = exif_data_new_from_file( foto->orig );

  if ( ed == NULL )
    return;

  ExifContent *e0 = ed->ifd[EXIF_IFD_0], *esub = ed->ifd[EXIF_IFD_EXIF];

  ExifEntry *e;

  foto->camera = "";
  if ( e = exif_content_get_entry( e0, EXIF_TAG_MAKE ) )
    foto->camera.get(  ) += exif_entry_get_value( e );
  if ( e = exif_content_get_entry( e0, EXIF_TAG_MODEL ) )
    foto->camera.get(  ) += wxString( foto->camera.get(  ).empty(  )? "" : " " ) + exif_entry_get_value( e );

  if ( foto->camera.get(  ).empty(  ) )
    foto->camera.reset(  );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_SHUTTER_SPEED_VALUE ) )
    foto->tempo_exp = rational < unsigned >( 1, ( unsigned ) pow( 2, rational_cast < double >( get_rational( e->data ) ) ) );

  else
if ( e = exif_content_get_entry( esub, EXIF_TAG_EXPOSURE_TIME ) )
  foto->tempo_exp = get_rational( e->data );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_FNUMBER ) )
    foto->f_stop = rational_cast < float >( get_rational( e->data ) );

  else
if ( e = exif_content_get_entry( esub, EXIF_TAG_APERTURE_VALUE ) )
  foto->f_stop = pow( sqrt( 2 ), rational_cast < double >( get_rational( e->data ) ) );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_ISO_SPEED_RATINGS ) )
    foto->iso = exif_get_short( e->data, THIS_BYTE_ORDER );

  wxString aux;

  if ( e = exif_content_get_entry( esub, EXIF_TAG_DATE_TIME_ORIGINAL ) )
    aux = exif_entry_get_value( e );
  else if ( e = exif_content_get_entry( esub, EXIF_TAG_DATE_TIME ) )
    aux = exif_entry_get_value( e );
  else if ( e = exif_content_get_entry( esub, EXIF_TAG_DATE_TIME_DIGITIZED ) )
    aux = exif_entry_get_value( e );
  if ( aux != "" )
  {
    wxDateTime x;

    x.ParseFormat( aux, "%Y:%m:%d %T" );
    foto->data_orig = x;
  }

  if ( e = exif_content_get_entry( esub, EXIF_TAG_FOCAL_LENGTH ) )
    foto->dist_focal = get_rational( e->data );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_FLASH ) )
    foto->flash = exif_get_short( e->data, THIS_BYTE_ORDER ) == 1 ? true : false;

  if ( e = exif_content_get_entry( esub, EXIF_TAG_SUBJECT_DISTANCE ) )
    foto->dist_assunto = get_srational( e->data );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_BRIGHTNESS_VALUE ) )
    foto->brilho = get_srational( e->data );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_EXPOSURE_PROGRAM ) )
    foto->prog_exp = ( foto_t::ProgramaExposicao ) exif_get_short( e->data, THIS_BYTE_ORDER );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_ORIENTATION ) )
    foto->orientacao = ( foto_t::Orientacao ) exif_get_short( e->data, THIS_BYTE_ORDER );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_LIGHT_SOURCE ) )
    foto->fonte_luz = ( foto_t::FonteLuz ) exif_get_short( e->data, THIS_BYTE_ORDER );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_FOCAL_PLANE_X_RESOLUTION ) )
  {
    rational < unsigned >x = get_rational( e->data );

    if ( e = exif_content_get_entry( esub, EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION ) )
    {
      rational < unsigned >y = get_rational( e->data );
      foto->megapixels = rational_cast < unsigned >( x * y );
    }
  }

  if ( e = exif_content_get_entry( esub, EXIF_TAG_EXPOSURE_BIAS_VALUE ) )
    foto->exp_bias = get_srational( e->data );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_METERING_MODE ) )
    foto->modo_medicao = ( foto_t::ModoMedicao ) exif_get_short( e->data, THIS_BYTE_ORDER );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_COMPRESSED_BITS_PER_PIXEL ) )
    foto->compressao = get_rational( e->data );

  if ( e = exif_content_get_entry( esub, EXIF_TAG_PIXEL_X_DIMENSION ) )
  {
    unsigned x = exif_get_long( e->data, THIS_BYTE_ORDER );

    if ( e = exif_content_get_entry( esub, EXIF_TAG_PIXEL_Y_DIMENSION ) )
    {
      unsigned y = exif_get_long( e->data, THIS_BYTE_ORDER );

      foto->dimensoes = wxSize( x, y );
    }
  }

  exif_data_free( ed );
#endif
}
