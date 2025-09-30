    
    #include "objects.h"

    objects * objects_construct(const configs * cfgs) {

        objects * objs;
        unsigned int iSink;

        printf("DEBUG: Starting objects_construct\n");
        fflush(stdout);

        objs = (objects *) malloc(sizeof(objects));

        printf("DEBUG: Allocated objects structure\n");
        fflush(stdout);

        // +----------------------------------------------------------+
        // | Construct                                                |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Raw                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Source                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing src_hops_mics_object\n");
                    fflush(stdout);
                    objs->src_hops_mics_object = src_hops_construct(cfgs->src_hops_mics_config, 
                                                                    cfgs->msg_hops_mics_raw_config);
                    printf("DEBUG: Completed src_hops_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_hops_mics_raw_object\n");
                    fflush(stdout);
                    objs->con_hops_mics_raw_object = con_hops_construct(1, cfgs->msg_hops_mics_raw_config);
                    printf("DEBUG: Completed con_hops_mics_raw_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Mapping                                              |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_mapping_mics_object\n");
                    fflush(stdout);
                    objs->mod_mapping_mics_object = mod_mapping_construct(cfgs->mod_mapping_mics_config, 
                                                                          cfgs->msg_hops_mics_map_config);
                    printf("DEBUG: Completed mod_mapping_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_hops_mics_map_object\n");
                    fflush(stdout);
                    objs->con_hops_mics_map_object = con_hops_construct(1, cfgs->msg_hops_mics_map_config);
                    printf("DEBUG: Completed con_hops_mics_map_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_resample_mics_object\n");
                    fflush(stdout);
                    objs->mod_resample_mics_object = mod_resample_construct(cfgs->mod_resample_mics_config, 
                                                                            cfgs->msg_hops_mics_map_config, 
                                                                            cfgs->msg_hops_mics_rs_config);
                    printf("DEBUG: Completed mod_resample_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_hops_mics_rs_object\n");
                    fflush(stdout);
                    objs->con_hops_mics_rs_object = con_hops_construct(2, cfgs->msg_hops_mics_rs_config);
                    printf("DEBUG: Completed con_hops_mics_rs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | STFT                                                 |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_stft_mics_object\n");
                    fflush(stdout);
                    objs->mod_stft_mics_object = mod_stft_construct(cfgs->mod_stft_mics_config, 
                                                                    cfgs->msg_hops_mics_rs_config, 
                                                                    cfgs->msg_spectra_mics_config);
                    printf("DEBUG: Completed mod_stft_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_spectra_mics_object\n");
                    fflush(stdout);
                    objs->con_spectra_mics_object = con_spectra_construct(3, cfgs->msg_spectra_mics_config);
                    printf("DEBUG: Completed con_spectra_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Noise                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_noise_mics_object\n");
                    fflush(stdout);
                    objs->mod_noise_mics_object = mod_noise_construct(cfgs->mod_noise_mics_config, 
                                                                      cfgs->msg_spectra_mics_config, 
                                                                      cfgs->msg_powers_mics_config);
                    printf("DEBUG: Completed mod_noise_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_powers_mics_object\n");
                    fflush(stdout);
                    objs->con_powers_mics_object = con_powers_construct(1, cfgs->msg_powers_mics_config);
                    printf("DEBUG: Completed con_powers_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSL                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  
                    
                    printf("DEBUG: Constructing mod_ssl_object\n");
                    fflush(stdout);
                    objs->mod_ssl_object = mod_ssl_construct(cfgs->mod_ssl_config, 
                                                             cfgs->msg_spectra_mics_config,
                                                             cfgs->msg_pots_ssl_config);
                    printf("DEBUG: Completed mod_ssl_object\n");
                    fflush(stdout);
                    
                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing snk_pots_ssl_object\n");
                    fflush(stdout);
                    objs->snk_pots_ssl_object = snk_pots_construct(cfgs->snk_pots_ssl_config, 
                                                                   cfgs->msg_pots_ssl_config);
                    printf("DEBUG: Completed snk_pots_ssl_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+ 

                printf("DEBUG: Constructing con_pots_ssl_object\n");
                fflush(stdout);
                objs->con_pots_ssl_object = con_pots_construct(2, cfgs->msg_pots_ssl_config);
                printf("DEBUG: Completed con_pots_ssl_object\n");
                fflush(stdout);

            // +------------------------------------------------------+
            // | Target                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Injector                                         |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing inj_targets_sst_object\n");
                    fflush(stdout);
                    objs->inj_targets_sst_object = inj_targets_construct(cfgs->inj_targets_sst_config, 
                                                                         cfgs->msg_hops_mics_rs_config, 
                                                                         cfgs->msg_targets_sst_config);
                    printf("DEBUG: Completed inj_targets_sst_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+              

                    printf("DEBUG: Constructing con_targets_sst_object\n");
                    fflush(stdout);
                    objs->con_targets_sst_object = con_targets_construct(1, cfgs->msg_targets_sst_config);
                    printf("DEBUG: Completed con_targets_sst_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SST                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_sst_object\n");
                    fflush(stdout);
                    objs->mod_sst_object = mod_sst_construct(cfgs->mod_sst_config, 
                                                             cfgs->mod_ssl_config, 
                                                             cfgs->msg_pots_ssl_config, 
                                                             cfgs->msg_targets_sst_config,
                                                             cfgs->msg_tracks_sst_config);
                    printf("DEBUG: Completed mod_sst_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing snk_tracks_sst_object\n");
                    fflush(stdout);
                    objs->snk_tracks_sst_object = snk_tracks_construct(cfgs->snk_tracks_sst_config, 
                                                                       cfgs->msg_tracks_sst_config);
                    printf("DEBUG: Completed snk_tracks_sst_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                    

                    printf("DEBUG: Constructing con_tracks_sst_object\n");
                    fflush(stdout);
                    objs->con_tracks_sst_object = con_tracks_construct(3, cfgs->msg_tracks_sst_config);
                    printf("DEBUG: Completed con_tracks_sst_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSS                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_sss_object\n");
                    fflush(stdout);
                    objs->mod_sss_object = mod_sss_construct(cfgs->mod_sss_config, 
                                                             cfgs->msg_tracks_sst_config, 
                                                             cfgs->msg_spectra_mics_config);
                    printf("DEBUG: Completed mod_sss_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                    

                    printf("DEBUG: Constructing con_spectra_seps_object\n");
                    fflush(stdout);
                    objs->con_spectra_seps_object = con_spectra_construct(1, cfgs->msg_spectra_seps_config);
                    printf("DEBUG: Completed con_spectra_seps_object\n");
                    fflush(stdout);
                    
                    printf("DEBUG: Constructing con_spectra_pfs_object\n");
                    fflush(stdout);
                    objs->con_spectra_pfs_object = con_spectra_construct(1, cfgs->msg_spectra_pfs_config);
                    printf("DEBUG: Completed con_spectra_pfs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | ISTFT                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_istft_seps_object\n");
                    fflush(stdout);
                    objs->mod_istft_seps_object = mod_istft_construct(cfgs->mod_istft_seps_config, 
                                                                      cfgs->msg_spectra_seps_config, 
                                                                      cfgs->msg_hops_seps_config);
                    printf("DEBUG: Completed mod_istft_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing mod_istft_pfs_object\n");
                    fflush(stdout);
                    objs->mod_istft_pfs_object = mod_istft_construct(cfgs->mod_istft_pfs_config, 
                                                                     cfgs->msg_spectra_pfs_config, 
                                                                     cfgs->msg_hops_pfs_config);
                    printf("DEBUG: Completed mod_istft_pfs_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                     

                    printf("DEBUG: Constructing con_hops_seps_object\n");
                    fflush(stdout);
                    objs->con_hops_seps_object = con_hops_construct(2, cfgs->msg_hops_seps_config);
                    printf("DEBUG: Completed con_hops_seps_object\n");
                    fflush(stdout);
                    
                    printf("DEBUG: Constructing con_hops_pfs_object\n");
                    fflush(stdout);
                    objs->con_hops_pfs_object = con_hops_construct(1, cfgs->msg_hops_pfs_config);
                    printf("DEBUG: Completed con_hops_pfs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_resample_seps_object\n");
                    fflush(stdout);
                    objs->mod_resample_seps_object = mod_resample_construct(cfgs->mod_resample_seps_config, 
                                                                            cfgs->msg_hops_seps_config, 
                                                                            cfgs->msg_hops_seps_rs_config);
                    printf("DEBUG: Completed mod_resample_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing mod_resample_pfs_object\n");
                    fflush(stdout);
                    objs->mod_resample_pfs_object = mod_resample_construct(cfgs->mod_resample_pfs_config, 
                                                                           cfgs->msg_hops_pfs_config, 
                                                                           cfgs->msg_hops_pfs_rs_config);
                    printf("DEBUG: Completed mod_resample_pfs_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_hops_seps_rs_object\n");
                    fflush(stdout);
                    objs->con_hops_seps_rs_object = con_hops_construct(1, cfgs->msg_hops_seps_rs_config);
                    printf("DEBUG: Completed con_hops_seps_rs_object\n");
                    fflush(stdout);
                    
                    printf("DEBUG: Constructing con_hops_pfs_rs_object\n");
                    fflush(stdout);
                    objs->con_hops_pfs_rs_object = con_hops_construct(1, cfgs->msg_hops_pfs_rs_config);
                    printf("DEBUG: Completed con_hops_pfs_rs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Volume                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_volume_seps_object\n");
                    fflush(stdout);
                    objs->mod_volume_seps_object = mod_volume_construct(cfgs->mod_volume_seps_config, 
                                                                        cfgs->msg_hops_seps_vol_config);
                    printf("DEBUG: Completed mod_volume_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing mod_volume_pfs_object\n");
                    fflush(stdout);
                    objs->mod_volume_pfs_object = mod_volume_construct(cfgs->mod_volume_pfs_config, 
                                                                       cfgs->msg_hops_pfs_vol_config);
                    printf("DEBUG: Completed mod_volume_pfs_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_hops_seps_vol_object\n");
                    fflush(stdout);
                    objs->con_hops_seps_vol_object = con_hops_construct(1, cfgs->msg_hops_seps_vol_config);
                    printf("DEBUG: Completed con_hops_seps_vol_object\n");
                    fflush(stdout);
                    
                    printf("DEBUG: Constructing con_hops_pfs_vol_object\n");
                    fflush(stdout);
                    objs->con_hops_pfs_vol_object = con_hops_construct(1, cfgs->msg_hops_pfs_vol_config);
                    printf("DEBUG: Completed con_hops_pfs_vol_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                     

                    printf("DEBUG: Constructing snk_hops_seps_vol_object\n");
                    fflush(stdout);
                    objs->snk_hops_seps_vol_object = snk_hops_construct(cfgs->snk_hops_seps_vol_config, 
                                                                        cfgs->msg_hops_seps_vol_config);
                    printf("DEBUG: Completed snk_hops_seps_vol_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing snk_hops_pfs_vol_object\n");
                    fflush(stdout);
                    objs->snk_hops_pfs_vol_object = snk_hops_construct(cfgs->snk_hops_pfs_vol_config, 
                                                                       cfgs->msg_hops_pfs_vol_config);
                    printf("DEBUG: Completed snk_hops_pfs_vol_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Classify                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing mod_classify_object\n");
                    fflush(stdout);
                    objs->mod_classify_object = mod_classify_construct(cfgs->mod_classify_config,
                                                                       cfgs->msg_hops_seps_config,
                                                                       cfgs->msg_tracks_sst_config,
                                                                       cfgs->msg_categories_config);
                    printf("DEBUG: Completed mod_classify_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing con_categories_object\n");
                    fflush(stdout);
                    objs->con_categories_object = con_categories_construct(1, cfgs->msg_categories_config);
                    printf("DEBUG: Completed con_categories_object\n");
                    fflush(stdout);
                
                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    printf("DEBUG: Constructing snk_categories_object\n");
                    fflush(stdout);
                    objs->snk_categories_object = snk_categories_construct(cfgs->snk_categories_config,
                                                                           cfgs->msg_categories_config);
                    printf("DEBUG: Completed snk_categories_object\n");
                    fflush(stdout);

        printf("DEBUG: Starting connection phase\n");
        fflush(stdout);

        // +----------------------------------------------------------+
        // | Connect                                                  |
        // +----------------------------------------------------------+              

            // +------------------------------------------------------+
            // | Raw                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Source                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting src_hops_mics_object\n");
                    fflush(stdout);
                    src_hops_connect(objs->src_hops_mics_object, 
                                     objs->con_hops_mics_raw_object->in);
                    printf("DEBUG: Completed src_hops_mics_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Mapping                                              |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_mapping_mics_object\n");
                    fflush(stdout);
                    mod_mapping_connect(objs->mod_mapping_mics_object, 
                                        objs->con_hops_mics_raw_object->outs[0], 
                                        objs->con_hops_mics_map_object->in);
                    printf("DEBUG: Completed mod_mapping_mics_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_resample_mics_object\n");
                    fflush(stdout);
                    mod_resample_connect(objs->mod_resample_mics_object, 
                                         objs->con_hops_mics_map_object->outs[0], 
                                         objs->con_hops_mics_rs_object->in);
                    printf("DEBUG: Completed mod_resample_mics_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | STFT                                                 |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_stft_mics_object\n");
                    fflush(stdout);
                    mod_stft_connect(objs->mod_stft_mics_object, 
                                     objs->con_hops_mics_rs_object->outs[0], 
                                     objs->con_spectra_mics_object->in);
                    printf("DEBUG: Completed mod_stft_mics_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Noise                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_noise_mics_object\n");
                    fflush(stdout);
                    mod_noise_connect(objs->mod_noise_mics_object,
                                      objs->con_spectra_mics_object->outs[0],
                                      objs->con_powers_mics_object->in);
                    printf("DEBUG: Completed mod_noise_mics_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSL                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_ssl_object\n");
                    fflush(stdout);
                    mod_ssl_connect(objs->mod_ssl_object, 
                                    objs->con_spectra_mics_object->outs[1], 
                                    objs->con_pots_ssl_object->in);
                    printf("DEBUG: Completed mod_ssl_object connection\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting snk_pots_ssl_object\n");
                    fflush(stdout);
                    snk_pots_connect(objs->snk_pots_ssl_object,
                                     objs->con_pots_ssl_object->outs[1]);
                    printf("DEBUG: Completed snk_pots_ssl_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Target                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Injector                                         |
                // +--------------------------------------------------+

                    printf("DEBUG: Connecting inj_targets_sst_object\n");
                    fflush(stdout);
                    inj_targets_connect(objs->inj_targets_sst_object,
                                        objs->con_hops_mics_rs_object->outs[1],
                                        objs->con_targets_sst_object->in);
                    printf("DEBUG: Completed inj_targets_sst_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SST                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_sst_object\n");
                    fflush(stdout);
                    mod_sst_connect(objs->mod_sst_object, 
                                    objs->con_pots_ssl_object->outs[0], 
                                    objs->con_targets_sst_object->outs[0],
                                    objs->con_tracks_sst_object->in);
                    printf("DEBUG: Completed mod_sst_object connection\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting snk_tracks_sst_object\n");
                    fflush(stdout);
                    snk_tracks_connect(objs->snk_tracks_sst_object,
                                       objs->con_tracks_sst_object->outs[2]);
                    printf("DEBUG: Completed snk_tracks_sst_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSS                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_sss_object\n");
                    fflush(stdout);
                    mod_sss_connect(objs->mod_sss_object,
                                    objs->con_spectra_mics_object->outs[2],
                                    objs->con_powers_mics_object->outs[0],
                                    objs->con_tracks_sst_object->outs[0],
                                    objs->con_spectra_seps_object->in,
                                    objs->con_spectra_pfs_object->in);
                    printf("DEBUG: Completed mod_sss_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | ISTFT                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+                      

                    printf("DEBUG: Connecting mod_istft_seps_object\n");
                    fflush(stdout);
                    mod_istft_connect(objs->mod_istft_seps_object,
                                      objs->con_spectra_seps_object->outs[0],
                                      objs->con_hops_seps_object->in);
                    printf("DEBUG: Completed mod_istft_seps_object connection\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting mod_istft_pfs_object\n");
                    fflush(stdout);
                    mod_istft_connect(objs->mod_istft_pfs_object,
                                      objs->con_spectra_pfs_object->outs[0],
                                      objs->con_hops_pfs_object->in);
                    printf("DEBUG: Completed mod_istft_pfs_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_resample_seps_object\n");
                    fflush(stdout);
                    mod_resample_connect(objs->mod_resample_seps_object,
                                         objs->con_hops_seps_object->outs[0],
                                         objs->con_hops_seps_rs_object->in);
                    printf("DEBUG: Completed mod_resample_seps_object connection\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting mod_resample_pfs_object\n");
                    fflush(stdout);
                    mod_resample_connect(objs->mod_resample_pfs_object,
                                         objs->con_hops_pfs_object->outs[0],
                                         objs->con_hops_pfs_rs_object->in);
                    printf("DEBUG: Completed mod_resample_pfs_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Volume                                               |
            // +------------------------------------------------------+ 

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_volume_seps_object\n");
                    fflush(stdout);
                    mod_volume_connect(objs->mod_volume_seps_object,
                                       objs->con_hops_seps_rs_object->outs[0],
                                       objs->con_hops_seps_vol_object->in);
                    printf("DEBUG: Completed mod_volume_seps_object connection\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting mod_volume_pfs_object\n");
                    fflush(stdout);
                    mod_volume_connect(objs->mod_volume_pfs_object,
                                       objs->con_hops_pfs_rs_object->outs[0],
                                       objs->con_hops_pfs_vol_object->in);
                    printf("DEBUG: Completed mod_volume_pfs_object connection\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    printf("DEBUG: Connecting snk_hops_seps_vol_object\n");
                    fflush(stdout);
                    snk_hops_connect(objs->snk_hops_seps_vol_object,
                                     objs->con_hops_seps_vol_object->outs[0]);
                    printf("DEBUG: Completed snk_hops_seps_vol_object connection\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting snk_hops_pfs_vol_object\n");
                    fflush(stdout);
                    snk_hops_connect(objs->snk_hops_pfs_vol_object,
                                     objs->con_hops_pfs_vol_object->outs[0]);
                    printf("DEBUG: Completed snk_hops_pfs_vol_object connection\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Classify                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting mod_classify_object\n");
                    fflush(stdout);
                    mod_classify_connect(objs->mod_classify_object,
                                         objs->con_hops_seps_object->outs[1],
                                         objs->con_tracks_sst_object->outs[1],
                                         objs->con_categories_object->in);
                    printf("DEBUG: Completed mod_classify_object connection\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting snk_categories_object\n");
                    fflush(stdout);
                    snk_categories_connect(objs->snk_categories_object,
                                           objs->con_categories_object->outs[0]);
                    printf("DEBUG: Completed snk_categories_object connection\n");
                    fflush(stdout);

        printf("DEBUG: Starting module enable phase\n");
        fflush(stdout);

        // +----------------------------------------------------------+
        // | Enable modules                                           |
        // +----------------------------------------------------------+  

            printf("DEBUG: Enabling basic modules\n");
            fflush(stdout);
            mod_mapping_enable(objs->mod_mapping_mics_object);
            mod_resample_enable(objs->mod_resample_mics_object);
            mod_stft_enable(objs->mod_stft_mics_object);
            printf("DEBUG: Completed basic module enables\n");
            fflush(stdout);

            if (cfgs->snk_pots_ssl_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling SSL module\n");
                fflush(stdout);
                mod_ssl_enable(objs->mod_ssl_object);
                printf("DEBUG: Completed SSL module enable\n");
                fflush(stdout);

            }

            if (cfgs->snk_tracks_sst_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling SSL and SST modules\n");
                fflush(stdout);
                mod_ssl_enable(objs->mod_ssl_object);
                mod_sst_enable(objs->mod_sst_object);
                printf("DEBUG: Completed SSL and SST module enables\n");
                fflush(stdout);

            }

            if (cfgs->snk_hops_seps_vol_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling separation pipeline modules\n");
                fflush(stdout);
                mod_ssl_enable(objs->mod_ssl_object);
                mod_sst_enable(objs->mod_sst_object);
                mod_noise_enable(objs->mod_noise_mics_object);
                mod_sss_enable(objs->mod_sss_object);
                mod_istft_enable(objs->mod_istft_seps_object);
                mod_resample_enable(objs->mod_resample_seps_object);
                mod_volume_enable(objs->mod_volume_seps_object);
                printf("DEBUG: Completed separation pipeline module enables\n");
                fflush(stdout);

            }

            if (cfgs->snk_hops_pfs_vol_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling postfilter pipeline modules\n");
                fflush(stdout);
                mod_ssl_enable(objs->mod_ssl_object);
                mod_sst_enable(objs->mod_sst_object);
                mod_noise_enable(objs->mod_noise_mics_object);
                mod_sss_enable(objs->mod_sss_object);
                mod_istft_enable(objs->mod_istft_pfs_object);
                mod_resample_enable(objs->mod_resample_pfs_object);
                mod_volume_enable(objs->mod_volume_pfs_object);
                printf("DEBUG: Completed postfilter pipeline module enables\n");
                fflush(stdout);

            }

            if (cfgs->snk_categories_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling classification pipeline modules\n");
                fflush(stdout);
                mod_ssl_enable(objs->mod_ssl_object);
                mod_sst_enable(objs->mod_sst_object);
                mod_noise_enable(objs->mod_noise_mics_object);
                mod_sss_enable(objs->mod_sss_object);
                mod_istft_enable(objs->mod_istft_seps_object);
                mod_classify_enable(objs->mod_classify_object);
                printf("DEBUG: Completed classification pipeline module enables\n");
                fflush(stdout);

            }

        printf("DEBUG: objects_construct completed successfully\n");
        fflush(stdout);

        return objs;

    }

    void objects_destroy(objects * objs) {

        unsigned int iSink;

        // +----------------------------------------------------------+
        // | Destroy                                                  |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Raw                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Source                                           |
                // +--------------------------------------------------+  

                    src_hops_destroy(objs->src_hops_mics_object);
                    
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_hops_destroy(objs->con_hops_mics_raw_object);

            // +------------------------------------------------------+
            // | Mapping                                              |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_mapping_destroy(objs->mod_mapping_mics_object);
                    
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_hops_destroy(objs->con_hops_mics_map_object);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_resample_destroy(objs->mod_resample_mics_object);
                                       
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_hops_destroy(objs->con_hops_mics_rs_object);

            // +------------------------------------------------------+
            // | STFT                                                 |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_stft_destroy(objs->mod_stft_mics_object);
                                        
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_spectra_destroy(objs->con_spectra_mics_object);

            // +------------------------------------------------------+
            // | Noise                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_noise_destroy(objs->mod_noise_mics_object);
                                        
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_powers_destroy(objs->con_powers_mics_object);

            // +------------------------------------------------------+
            // | SSL                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_ssl_destroy(objs->mod_ssl_object);
                                       
                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    snk_pots_destroy(objs->snk_pots_ssl_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_pots_destroy(objs->con_pots_ssl_object);

            // +------------------------------------------------------+
            // | Target                                               |
            // +------------------------------------------------------+ 

                // +--------------------------------------------------+
                // | Injector                                         |
                // +--------------------------------------------------+  

                    inj_targets_destroy(objs->inj_targets_sst_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_targets_destroy(objs->con_targets_sst_object);

            // +------------------------------------------------------+
            // | SST                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_sst_destroy(objs->mod_sst_object);
                                        
                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    snk_tracks_destroy(objs->snk_tracks_sst_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_tracks_destroy(objs->con_tracks_sst_object);

            // +------------------------------------------------------+
            // | SSS                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_sss_destroy(objs->mod_sss_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_spectra_destroy(objs->con_spectra_seps_object);
                    con_spectra_destroy(objs->con_spectra_pfs_object);                    

            // +------------------------------------------------------+
            // | ISTFT                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+                      

                    mod_istft_destroy(objs->mod_istft_seps_object);
                    mod_istft_destroy(objs->mod_istft_pfs_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_hops_destroy(objs->con_hops_seps_object);
                    con_hops_destroy(objs->con_hops_pfs_object);                     

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_resample_destroy(objs->mod_resample_seps_object);
                    mod_resample_destroy(objs->mod_resample_pfs_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_hops_destroy(objs->con_hops_seps_rs_object);
                    con_hops_destroy(objs->con_hops_pfs_rs_object);                     

            // +------------------------------------------------------+
            // | Volume                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_volume_destroy(objs->mod_volume_seps_object);
                    mod_volume_destroy(objs->mod_volume_pfs_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_hops_destroy(objs->con_hops_seps_vol_object);
                    con_hops_destroy(objs->con_hops_pfs_vol_object);       

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    snk_hops_destroy(objs->snk_hops_seps_vol_object);
                    snk_hops_destroy(objs->snk_hops_pfs_vol_object);

            // +------------------------------------------------------+
            // | Classify                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    mod_classify_destroy(objs->mod_classify_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    con_categories_destroy(objs->con_categories_object);
                
                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    snk_categories_destroy(objs->snk_categories_object);

        free((void *) objs);

    }

    aobjects * aobjects_construct(const configs * cfgs) {

        aobjects * objs;
        unsigned int iSink;

        printf("DEBUG: Starting aobjects_construct\n");
        fflush(stdout);

        objs = (aobjects *) malloc(sizeof(aobjects));

        objs->nMessages = 100;

        printf("DEBUG: Allocated aobjects structure\n");
        fflush(stdout);

        // +----------------------------------------------------------+
        // | Construct                                                |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Raw                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Source                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing asrc_hops_mics_object\n");
                    fflush(stdout);
                    objs->asrc_hops_mics_object = asrc_hops_construct(cfgs->src_hops_mics_config, 
                                                                      cfgs->msg_hops_mics_raw_config);
                    printf("DEBUG: Completed asrc_hops_mics_object\n");
                    fflush(stdout);
                
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+

                    printf("DEBUG: Constructing acon_hops_mics_raw_object\n");
                    fflush(stdout);
                    objs->acon_hops_mics_raw_object = acon_hops_construct(1, objs->nMessages, cfgs->msg_hops_mics_raw_config);  
                    printf("DEBUG: Completed acon_hops_mics_raw_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Mapping                                              |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_mapping_mics_object\n");
                    fflush(stdout);
                    objs->amod_mapping_mics_object = amod_mapping_construct(cfgs->mod_mapping_mics_config, 
                                                                            cfgs->msg_hops_mics_map_config);
                    printf("DEBUG: Completed amod_mapping_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+

                    printf("DEBUG: Constructing acon_hops_mics_map_object\n");
                    fflush(stdout);
                    objs->acon_hops_mics_map_object = acon_hops_construct(1, objs->nMessages, cfgs->msg_hops_mics_map_config);
                    printf("DEBUG: Completed acon_hops_mics_map_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_resample_mics_object\n");
                    fflush(stdout);
                    objs->amod_resample_mics_object = amod_resample_construct(cfgs->mod_resample_mics_config, 
                                                                              cfgs->msg_hops_mics_map_config, 
                                                                              cfgs->msg_hops_mics_rs_config);
                    printf("DEBUG: Completed amod_resample_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+

                    printf("DEBUG: Constructing acon_hops_mics_rs_object\n");
                    fflush(stdout);
                    objs->acon_hops_mics_rs_object = acon_hops_construct(2, objs->nMessages, cfgs->msg_hops_mics_rs_config);
                    printf("DEBUG: Completed acon_hops_mics_rs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | STFT                                                 |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_stft_mics_object\n");
                    fflush(stdout);
                    objs->amod_stft_mics_object = amod_stft_construct(cfgs->mod_stft_mics_config, 
                                                                      cfgs->msg_hops_mics_rs_config, 
                                                                      cfgs->msg_spectra_mics_config);
                    printf("DEBUG: Completed amod_stft_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+
                    
                    printf("DEBUG: Constructing acon_spectra_mics_object\n");
                    fflush(stdout);
                    objs->acon_spectra_mics_object = acon_spectra_construct(3, objs->nMessages, cfgs->msg_spectra_mics_config);
                    printf("DEBUG: Completed acon_spectra_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Noise                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_noise_mics_object\n");
                    fflush(stdout);
                    objs->amod_noise_mics_object = amod_noise_construct(cfgs->mod_noise_mics_config, 
                                                                        cfgs->msg_spectra_mics_config, 
                                                                        cfgs->msg_powers_mics_config);
                    printf("DEBUG: Completed amod_noise_mics_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+
                    
                    printf("DEBUG: Constructing acon_powers_mics_object\n");
                    fflush(stdout);
                    objs->acon_powers_mics_object = acon_powers_construct(1, objs->nMessages, cfgs->msg_powers_mics_config);
                    printf("DEBUG: Completed acon_powers_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSL                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_ssl_object\n");
                    fflush(stdout);
                    objs->amod_ssl_object = amod_ssl_construct(cfgs->mod_ssl_config, 
                                                               cfgs->msg_spectra_mics_config, 
                                                               cfgs->msg_pots_ssl_config);
                    printf("DEBUG: Completed amod_ssl_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing asnk_pots_ssl_object\n");
                    fflush(stdout);
                    objs->asnk_pots_ssl_object = asnk_pots_construct(cfgs->snk_pots_ssl_config, 
                                                                     cfgs->msg_pots_ssl_config);
                    printf("DEBUG: Completed asnk_pots_ssl_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+

                    printf("DEBUG: Constructing acon_pots_ssl_object\n");
                    fflush(stdout);
                    objs->acon_pots_ssl_object = acon_pots_construct(2, objs->nMessages, cfgs->msg_pots_ssl_config);
                    printf("DEBUG: Completed acon_pots_ssl_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Target                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Injector                                         |
                // +--------------------------------------------------+ 

                    printf("DEBUG: Constructing ainj_targets_sst_object\n");
                    fflush(stdout);
                    objs->ainj_targets_sst_object = ainj_targets_construct(cfgs->inj_targets_sst_config, 
                                                                           cfgs->msg_hops_mics_rs_config, 
                                                                           cfgs->msg_targets_sst_config);
                    printf("DEBUG: Completed ainj_targets_sst_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+              

                    printf("DEBUG: Constructing acon_targets_sst_object\n");
                    fflush(stdout);
                    objs->acon_targets_sst_object = acon_targets_construct(1, objs->nMessages, cfgs->msg_targets_sst_config);
                    printf("DEBUG: Completed acon_targets_sst_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SST                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_sst_object\n");
                    fflush(stdout);
                    objs->amod_sst_object = amod_sst_construct(cfgs->mod_sst_config, 
                                                               cfgs->mod_ssl_config, 
                                                               cfgs->msg_pots_ssl_config, 
                                                               cfgs->msg_targets_sst_config,
                                                               cfgs->msg_tracks_sst_config);
                    printf("DEBUG: Completed amod_sst_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing asnk_tracks_sst_object\n");
                    fflush(stdout);
                    objs->asnk_tracks_sst_object = asnk_tracks_construct(cfgs->snk_tracks_sst_config, 
                                                                         cfgs->msg_tracks_sst_config);
                    printf("DEBUG: Completed asnk_tracks_sst_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+

                    printf("DEBUG: Constructing acon_tracks_sst_object\n");
                    fflush(stdout);
                    objs->acon_tracks_sst_object = acon_tracks_construct(3, objs->nMessages, cfgs->msg_tracks_sst_config);
                    printf("DEBUG: Completed acon_tracks_sst_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSS                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_sss_object\n");
                    fflush(stdout);
                    objs->amod_sss_object = amod_sss_construct(cfgs->mod_sss_config, 
                                                               cfgs->msg_tracks_sst_config, 
                                                               cfgs->msg_spectra_mics_config);
                    printf("DEBUG: Completed amod_sss_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                    

                    printf("DEBUG: Constructing acon_spectra_seps_object\n");
                    fflush(stdout);
                    objs->acon_spectra_seps_object = acon_spectra_construct(1, objs->nMessages, cfgs->msg_spectra_seps_config);
                    printf("DEBUG: Completed acon_spectra_seps_object\n");
                    fflush(stdout);
                    
                    printf("DEBUG: Constructing acon_spectra_pfs_object\n");
                    fflush(stdout);
                    objs->acon_spectra_pfs_object = acon_spectra_construct(1, objs->nMessages, cfgs->msg_spectra_pfs_config);
                    printf("DEBUG: Completed acon_spectra_pfs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | ISTFT                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_istft_seps_object\n");
                    fflush(stdout);
                    objs->amod_istft_seps_object = amod_istft_construct(cfgs->mod_istft_seps_config, 
                                                                        cfgs->msg_spectra_seps_config, 
                                                                        cfgs->msg_hops_seps_config);
                    printf("DEBUG: Completed amod_istft_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing amod_istft_pfs_object\n");
                    fflush(stdout);
                    objs->amod_istft_pfs_object = amod_istft_construct(cfgs->mod_istft_pfs_config, 
                                                                       cfgs->msg_spectra_pfs_config, 
                                                                       cfgs->msg_hops_pfs_config);
                    printf("DEBUG: Completed amod_istft_pfs_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+                     

                    printf("DEBUG: Constructing acon_hops_seps_object\n");
                    fflush(stdout);
                    objs->acon_hops_seps_object = acon_hops_construct(2, objs->nMessages, cfgs->msg_hops_seps_config);
                    printf("DEBUG: Completed acon_hops_seps_object\n");
                    fflush(stdout);
                    
                    printf("DEBUG: Constructing acon_hops_pfs_object\n");
                    fflush(stdout);
                    objs->acon_hops_pfs_object = acon_hops_construct(1, objs->nMessages, cfgs->msg_hops_pfs_config);
                    printf("DEBUG: Completed acon_hops_pfs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_resample_seps_object\n");
                    fflush(stdout);
                    objs->amod_resample_seps_object = amod_resample_construct(cfgs->mod_resample_seps_config, 
                                                                              cfgs->msg_hops_seps_config, 
                                                                              cfgs->msg_hops_seps_rs_config);
                    printf("DEBUG: Completed amod_resample_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing amod_resample_pfs_object\n");
                    fflush(stdout);
                    objs->amod_resample_pfs_object = amod_resample_construct(cfgs->mod_resample_pfs_config, 
                                                                             cfgs->msg_hops_pfs_config, 
                                                                             cfgs->msg_hops_pfs_rs_config);                                                                                   
                    printf("DEBUG: Completed amod_resample_pfs_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing acon_hops_seps_rs_object\n");
                    fflush(stdout);
                    objs->acon_hops_seps_rs_object = acon_hops_construct(1, objs->nMessages, cfgs->msg_hops_seps_rs_config);
                    printf("DEBUG: Completed acon_hops_seps_rs_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing acon_hops_pfs_rs_object\n");
                    fflush(stdout);
                    objs->acon_hops_pfs_rs_object = acon_hops_construct(1, objs->nMessages, cfgs->msg_hops_pfs_rs_config);
                    printf("DEBUG: Completed acon_hops_pfs_rs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Volume                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_volume_seps_object\n");
                    fflush(stdout);
                    objs->amod_volume_seps_object = amod_volume_construct(cfgs->mod_volume_seps_config, 
                                                                          cfgs->msg_hops_seps_vol_config);
                    printf("DEBUG: Completed amod_volume_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing amod_volume_pfs_object\n");
                    fflush(stdout);
                    objs->amod_volume_pfs_object = amod_volume_construct(cfgs->mod_volume_pfs_config, 
                                                                         cfgs->msg_hops_pfs_vol_config);                                                                                   
                    printf("DEBUG: Completed amod_volume_pfs_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing acon_hops_seps_vol_object\n");
                    fflush(stdout);
                    objs->acon_hops_seps_vol_object = acon_hops_construct(1, objs->nMessages, cfgs->msg_hops_seps_vol_config);
                    printf("DEBUG: Completed acon_hops_seps_vol_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing acon_hops_pfs_vol_object\n");
                    fflush(stdout);
                    objs->acon_hops_pfs_vol_object = acon_hops_construct(1, objs->nMessages, cfgs->msg_hops_pfs_vol_config);
                    printf("DEBUG: Completed acon_hops_pfs_vol_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                     

                    printf("DEBUG: Constructing asnk_hops_seps_vol_object\n");
                    fflush(stdout);
                    objs->asnk_hops_seps_vol_object = asnk_hops_construct(cfgs->snk_hops_seps_vol_config, 
                                                                          cfgs->msg_hops_seps_vol_config);
                    printf("DEBUG: Completed asnk_hops_seps_vol_object\n");
                    fflush(stdout);

                    printf("DEBUG: Constructing asnk_hops_pfs_vol_object\n");
                    fflush(stdout);
                    objs->asnk_hops_pfs_vol_object = asnk_hops_construct(cfgs->snk_hops_pfs_vol_config, 
                                                                         cfgs->msg_hops_pfs_vol_config);
                    printf("DEBUG: Completed asnk_hops_pfs_vol_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Classify                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing amod_classify_object\n");
                    fflush(stdout);
                    objs->amod_classify_object = amod_classify_construct(cfgs->mod_classify_config,
                                                                         cfgs->msg_hops_seps_config,
                                                                         cfgs->msg_tracks_sst_config,
                                                                         cfgs->msg_categories_config);
                    printf("DEBUG: Completed amod_classify_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    printf("DEBUG: Constructing acon_categories_object\n");
                    fflush(stdout);
                    objs->acon_categories_object = acon_categories_construct(1, objs->nMessages, cfgs->msg_categories_config);
                    printf("DEBUG: Completed acon_categories_object\n");
                    fflush(stdout);
                
                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    printf("DEBUG: Constructing asnk_categories_object\n");
                    fflush(stdout);
                    objs->asnk_categories_object = asnk_categories_construct(cfgs->snk_categories_config,
                                                                             cfgs->msg_categories_config);
                    printf("DEBUG: Completed asnk_categories_object\n");
                    fflush(stdout);

        printf("DEBUG: Starting connection phase\n");
        fflush(stdout);

        // +----------------------------------------------------------+
        // | Connect                                                  |
        // +----------------------------------------------------------+  

            // +------------------------------------------------------+
            // | Raw                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Source                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting asrc_hops_mics_object\n");
                    fflush(stdout);
                    asrc_hops_connect(objs->asrc_hops_mics_object, 
                                      objs->acon_hops_mics_raw_object->in);
                    printf("DEBUG: Completed connecting asrc_hops_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Mapping                                              |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_mapping_mics_object\n");
                    fflush(stdout);
                    amod_mapping_connect(objs->amod_mapping_mics_object, 
                                         objs->acon_hops_mics_raw_object->outs[0], 
                                         objs->acon_hops_mics_map_object->in);
                    printf("DEBUG: Completed connecting amod_mapping_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_resample_mics_object\n");
                    fflush(stdout);
                    amod_resample_connect(objs->amod_resample_mics_object, 
                                          objs->acon_hops_mics_map_object->outs[0], 
                                          objs->acon_hops_mics_rs_object->in);
                    printf("DEBUG: Completed connecting amod_resample_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | STFT                                                 |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_stft_mics_object\n");
                    fflush(stdout);
                    amod_stft_connect(objs->amod_stft_mics_object, 
                                      objs->acon_hops_mics_rs_object->outs[0], 
                                      objs->acon_spectra_mics_object->in);
                    printf("DEBUG: Completed connecting amod_stft_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Noise                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_noise_mics_object\n");
                    fflush(stdout);
                    amod_noise_connect(objs->amod_noise_mics_object,
                                       objs->acon_spectra_mics_object->outs[0],
                                       objs->acon_powers_mics_object->in);
                    printf("DEBUG: Completed connecting amod_noise_mics_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSL                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_ssl_object\n");
                    fflush(stdout);
                    amod_ssl_connect(objs->amod_ssl_object, 
                                     objs->acon_spectra_mics_object->outs[1], 
                                     objs->acon_pots_ssl_object->in);
                    printf("DEBUG: Completed connecting amod_ssl_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting asnk_pots_ssl_object\n");
                    fflush(stdout);
                    asnk_pots_connect(objs->asnk_pots_ssl_object,
                                      objs->acon_pots_ssl_object->outs[1]);
                    printf("DEBUG: Completed connecting asnk_pots_ssl_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Target                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Injector                                         |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting ainj_targets_sst_object\n");
                    fflush(stdout);
                    ainj_targets_connect(objs->ainj_targets_sst_object,
                                         objs->acon_hops_mics_rs_object->outs[1],
                                         objs->acon_targets_sst_object->in);
                    printf("DEBUG: Completed connecting ainj_targets_sst_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SST                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_sst_object\n");
                    fflush(stdout);
                    amod_sst_connect(objs->amod_sst_object, 
                                     objs->acon_pots_ssl_object->outs[0], 
                                     objs->acon_targets_sst_object->outs[0],
                                     objs->acon_tracks_sst_object->in);
                    printf("DEBUG: Completed connecting amod_sst_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting asnk_tracks_sst_object\n");
                    fflush(stdout);
                    asnk_tracks_connect(objs->asnk_tracks_sst_object,
                                        objs->acon_tracks_sst_object->outs[2]);
                    printf("DEBUG: Completed connecting asnk_tracks_sst_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | SSS                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_sss_object\n");
                    fflush(stdout);
                    amod_sss_connect(objs->amod_sss_object,
                                     objs->acon_spectra_mics_object->outs[2],
                                     objs->acon_powers_mics_object->outs[0],
                                     objs->acon_tracks_sst_object->outs[0],
                                     objs->acon_spectra_seps_object->in,
                                     objs->acon_spectra_pfs_object->in);
                    printf("DEBUG: Completed connecting amod_sss_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | ISTFT                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+                      

                    printf("DEBUG: Connecting amod_istft_seps_object\n");
                    fflush(stdout);
                    amod_istft_connect(objs->amod_istft_seps_object,
                                       objs->acon_spectra_seps_object->outs[0],
                                       objs->acon_hops_seps_object->in);
                    printf("DEBUG: Completed connecting amod_istft_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting amod_istft_pfs_object\n");
                    fflush(stdout);
                    amod_istft_connect(objs->amod_istft_pfs_object,
                                       objs->acon_spectra_pfs_object->outs[0],
                                       objs->acon_hops_pfs_object->in);
                    printf("DEBUG: Completed connecting amod_istft_pfs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_resample_seps_object\n");
                    fflush(stdout);
                    amod_resample_connect(objs->amod_resample_seps_object,
                                          objs->acon_hops_seps_object->outs[0],
                                          objs->acon_hops_seps_rs_object->in);
                    printf("DEBUG: Completed connecting amod_resample_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting amod_resample_pfs_object\n");
                    fflush(stdout);
                    amod_resample_connect(objs->amod_resample_pfs_object,
                                          objs->acon_hops_pfs_object->outs[0],
                                          objs->acon_hops_pfs_rs_object->in);
                    printf("DEBUG: Completed connecting amod_resample_pfs_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Volume                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_volume_seps_object\n");
                    fflush(stdout);
                    amod_volume_connect(objs->amod_volume_seps_object,
                                        objs->acon_hops_seps_rs_object->outs[0],
                                        objs->acon_hops_seps_vol_object->in);
                    printf("DEBUG: Completed connecting amod_volume_seps_object\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting amod_volume_pfs_object\n");
                    fflush(stdout);
                    amod_volume_connect(objs->amod_volume_pfs_object,
                                        objs->acon_hops_pfs_rs_object->outs[0],
                                        objs->acon_hops_pfs_vol_object->in);
                    printf("DEBUG: Completed connecting amod_volume_pfs_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    printf("DEBUG: Connecting asnk_hops_seps_vol_object\n");
                    fflush(stdout);
                    asnk_hops_connect(objs->asnk_hops_seps_vol_object,
                                      objs->acon_hops_seps_vol_object->outs[0]);
                    printf("DEBUG: Completed connecting asnk_hops_seps_vol_object\n");
                    fflush(stdout);

                    printf("DEBUG: Connecting asnk_hops_pfs_vol_object\n");
                    fflush(stdout);
                    asnk_hops_connect(objs->asnk_hops_pfs_vol_object,
                                      objs->acon_hops_pfs_vol_object->outs[0]);
                    printf("DEBUG: Completed connecting asnk_hops_pfs_vol_object\n");
                    fflush(stdout);

            // +------------------------------------------------------+
            // | Classify                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting amod_classify_object\n");
                    fflush(stdout);
                    amod_classify_connect(objs->amod_classify_object,
                                          objs->acon_hops_seps_object->outs[1],
                                          objs->acon_tracks_sst_object->outs[1],
                                          objs->acon_categories_object->in);
                    printf("DEBUG: Completed connecting amod_classify_object\n");
                    fflush(stdout);

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+  

                    printf("DEBUG: Connecting asnk_categories_object\n");
                    fflush(stdout);
                    asnk_categories_connect(objs->asnk_categories_object,
                                            objs->acon_categories_object->outs[0]);
                    printf("DEBUG: Completed connecting asnk_categories_object\n");
                    fflush(stdout);

        printf("DEBUG: Starting module enable phase\n");
        fflush(stdout);

        // +----------------------------------------------------------+
        // | Enable modules                                           |
        // +----------------------------------------------------------+  

            printf("DEBUG: Enabling basic modules\n");
            fflush(stdout);
            amod_mapping_enable(objs->amod_mapping_mics_object);
            amod_resample_enable(objs->amod_resample_mics_object);
            amod_stft_enable(objs->amod_stft_mics_object);
            printf("DEBUG: Completed enabling basic modules\n");
            fflush(stdout);

            if (cfgs->snk_pots_ssl_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling SSL module\n");
                fflush(stdout);
                amod_ssl_enable(objs->amod_ssl_object);
                printf("DEBUG: Completed enabling SSL module\n");
                fflush(stdout);

            }

            if (cfgs->snk_tracks_sst_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling SSL and SST modules for tracks\n");
                fflush(stdout);
                amod_ssl_enable(objs->amod_ssl_object);
                amod_sst_enable(objs->amod_sst_object);
                printf("DEBUG: Completed enabling SSL and SST modules for tracks\n");
                fflush(stdout);

            }

            if (cfgs->snk_hops_seps_vol_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling modules for separated audio\n");
                fflush(stdout);
                amod_ssl_enable(objs->amod_ssl_object);
                amod_sst_enable(objs->amod_sst_object);
                amod_noise_enable(objs->amod_noise_mics_object);
                amod_sss_enable(objs->amod_sss_object);
                amod_istft_enable(objs->amod_istft_seps_object);
                amod_resample_enable(objs->amod_resample_seps_object);
                amod_volume_enable(objs->amod_volume_seps_object);
                printf("DEBUG: Completed enabling modules for separated audio\n");
                fflush(stdout);

            }

            if (cfgs->snk_hops_pfs_vol_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling modules for postfiltered audio\n");
                fflush(stdout);
                amod_ssl_enable(objs->amod_ssl_object);
                amod_sst_enable(objs->amod_sst_object);
                amod_noise_enable(objs->amod_noise_mics_object);
                amod_sss_enable(objs->amod_sss_object);
                amod_istft_enable(objs->amod_istft_pfs_object);
                amod_resample_enable(objs->amod_resample_pfs_object);
                amod_volume_enable(objs->amod_volume_pfs_object);
                printf("DEBUG: Completed enabling modules for postfiltered audio\n");
                fflush(stdout);

            }

            if (cfgs->snk_categories_config->interface->type != interface_blackhole) {

                printf("DEBUG: Enabling modules for classification\n");
                fflush(stdout);
                amod_ssl_enable(objs->amod_ssl_object);
                amod_sst_enable(objs->amod_sst_object);
                amod_noise_enable(objs->amod_noise_mics_object);
                amod_sss_enable(objs->amod_sss_object);
                amod_istft_enable(objs->amod_istft_seps_object);
                amod_classify_enable(objs->amod_classify_object);
                printf("DEBUG: Completed enabling modules for classification\n");
                fflush(stdout);

            }

        printf("DEBUG: aobjects_construct completed successfully\n");
        fflush(stdout);

        return objs;

    }

    void aobjects_destroy(aobjects * objs) {

        unsigned int iSink;

        // +----------------------------------------------------------+
        // | Destroy                                                  |
        // +----------------------------------------------------------+

            // +------------------------------------------------------+
            // | Raw                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Source                                           |
                // +--------------------------------------------------+  

                    asrc_hops_destroy(objs->asrc_hops_mics_object);
                    
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_hops_destroy(objs->acon_hops_mics_raw_object);

            // +------------------------------------------------------+
            // | Mapping                                              |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_mapping_destroy(objs->amod_mapping_mics_object);
                    
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_hops_destroy(objs->acon_hops_mics_map_object);             

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_resample_destroy(objs->amod_resample_mics_object);
                    
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_hops_destroy(objs->acon_hops_mics_rs_object);    

            // +------------------------------------------------------+
            // | STFT                                                 |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_stft_destroy(objs->amod_stft_mics_object);
                    
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_spectra_destroy(objs->acon_spectra_mics_object);   

            // +------------------------------------------------------+
            // | Noise                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_noise_destroy(objs->amod_noise_mics_object);
                    
                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_powers_destroy(objs->acon_powers_mics_object);  

            // +------------------------------------------------------+
            // | SSL                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_ssl_destroy(objs->amod_ssl_object);
                    
                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    asnk_pots_destroy(objs->asnk_pots_ssl_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_pots_destroy(objs->acon_pots_ssl_object);   

            // +------------------------------------------------------+
            // | Target                                               |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Injector                                         |
                // +--------------------------------------------------+  

                    ainj_targets_destroy(objs->ainj_targets_sst_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_targets_destroy(objs->acon_targets_sst_object);

            // +------------------------------------------------------+
            // | SST                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_sst_destroy(objs->amod_sst_object);
                    
                // +--------------------------------------------------+
                // | Sinks                                            |
                // +--------------------------------------------------+  

                    asnk_tracks_destroy(objs->asnk_tracks_sst_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_tracks_destroy(objs->acon_tracks_sst_object);  

            // +------------------------------------------------------+
            // | SSS                                                  |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_sss_destroy(objs->amod_sss_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_spectra_destroy(objs->acon_spectra_seps_object);
                    acon_spectra_destroy(objs->acon_spectra_pfs_object);                    

            // +------------------------------------------------------+
            // | ISTFT                                                |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+                      

                    amod_istft_destroy(objs->amod_istft_seps_object);
                    amod_istft_destroy(objs->amod_istft_pfs_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_hops_destroy(objs->acon_hops_seps_object);
                    acon_hops_destroy(objs->acon_hops_pfs_object);                     

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_resample_destroy(objs->amod_resample_seps_object);
                    amod_resample_destroy(objs->amod_resample_pfs_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_hops_destroy(objs->acon_hops_seps_rs_object);
                    acon_hops_destroy(objs->acon_hops_pfs_rs_object);                     

            // +------------------------------------------------------+
            // | Resample                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_volume_destroy(objs->amod_volume_seps_object);
                    amod_volume_destroy(objs->amod_volume_pfs_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_hops_destroy(objs->acon_hops_seps_vol_object);
                    acon_hops_destroy(objs->acon_hops_pfs_vol_object);  

                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    asnk_hops_destroy(objs->asnk_hops_seps_vol_object);
                    asnk_hops_destroy(objs->asnk_hops_pfs_vol_object);

            // +------------------------------------------------------+
            // | Classify                                             |
            // +------------------------------------------------------+  

                // +--------------------------------------------------+
                // | Module                                           |
                // +--------------------------------------------------+  

                    amod_classify_destroy(objs->amod_classify_object);

                // +--------------------------------------------------+
                // | Connector                                        |
                // +--------------------------------------------------+  

                    acon_categories_destroy(objs->acon_categories_object);
                
                // +--------------------------------------------------+
                // | Sink                                             |
                // +--------------------------------------------------+                      

                    asnk_categories_destroy(objs->asnk_categories_object);

        free((void *) objs);

    }