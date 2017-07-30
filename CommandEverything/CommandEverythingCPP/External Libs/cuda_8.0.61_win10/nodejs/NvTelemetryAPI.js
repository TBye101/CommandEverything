(function(){

    var backendAPI;
    var Logger;
    module.exports = function(NvBackendAPI, logger){
        backendAPI = NvBackendAPI;
        Logger = logger;

        return{
            sendDownloadSuccess: function(stat){
                try{
                    backendAPI.reportHttpSuccess(function reply(err, data){
                        if(err){
                            Logger.error(err);
                        }
                    }, stat);
                }catch(e){
                    throw(e);
                }
            },
            sendDownloadFailure: function(stat){
                try{
                    backendAPI.reportHttpFailure(function reply(err, data){
                        if(err){
                            Logger.error(err);
                        }
                    }, stat);
                }catch(e){
                    throw(e);
                }
            }
        };
    };
} ());