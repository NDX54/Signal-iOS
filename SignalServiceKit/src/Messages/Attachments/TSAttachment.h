//
//  Copyright (c) 2019 Open Whisper Systems. All rights reserved.
//

#import "BaseModel.h"

NS_ASSUME_NONNULL_BEGIN

@class SDSAnyReadTransaction;
@class TSAttachmentPointer;
@class TSMessage;

typedef NS_ENUM(NSUInteger, TSAttachmentType) {
    TSAttachmentTypeDefault = 0,
    TSAttachmentTypeVoiceMessage = 1,
};

@interface TSAttachment : BaseModel {

@protected
    NSString *_contentType;
}

// TSAttachment is a base class for TSAttachmentPointer (a yet-to-be-downloaded
// incoming attachment) and TSAttachmentStream (an outgoing or already-downloaded
// incoming attachment).
//
// The attachmentSchemaVersion and serverId properties only apply to
// TSAttachmentPointer, which can be distinguished by the isDownloaded
// property.
@property (atomic, readwrite) UInt64 serverId;
@property (atomic, readwrite, nullable) NSData *encryptionKey;
@property (nonatomic, readonly) NSString *contentType;
@property (nonatomic) TSAttachmentType attachmentType;

// Though now required, may incorrectly be 0 on legacy attachments.
@property (nonatomic, readonly) UInt32 byteCount;

// Represents the "source" filename sent or received in the protos,
// not the filename on disk.
@property (nonatomic, readonly, nullable) NSString *sourceFilename;

@property (nonatomic, readonly, nullable) NSString *blurHash;

#pragma mark - Media Album

@property (nonatomic, readonly, nullable) NSString *caption;
@property (nonatomic, readonly, nullable) NSString *albumMessageId;

- (nullable TSMessage *)fetchAlbumMessageWithTransaction:(SDSAnyReadTransaction *)transaction
    NS_SWIFT_NAME(fetchAlbumMessage(transaction:));

// `migrateAlbumMessageId` is only used in the migration to the new multi-attachment message scheme,
// and shouldn't be used as a general purpose setter. Instead, `albumMessageId` should be passed as
// an initializer param.
- (void)migrateAlbumMessageId:(NSString *)albumMesssageId;

#pragma mark -

// This constructor is used for new instances of TSAttachmentPointer,
// i.e. undownloaded incoming attachments.
- (instancetype)initWithServerId:(UInt64)serverId
                   encryptionKey:(NSData *)encryptionKey
                       byteCount:(UInt32)byteCount
                     contentType:(NSString *)contentType
                  sourceFilename:(nullable NSString *)sourceFilename
                         caption:(nullable NSString *)caption
                  albumMessageId:(nullable NSString *)albumMessageId
                        blurHash:(nullable NSString *)blurHash;

// This constructor is used for new instances of TSAttachmentPointer,
// i.e. undownloaded restoring attachments.
- (instancetype)initForRestoreWithUniqueId:(NSString *)uniqueId
                               contentType:(NSString *)contentType
                            sourceFilename:(nullable NSString *)sourceFilename
                                   caption:(nullable NSString *)caption
                            albumMessageId:(nullable NSString *)albumMessageId;

// This constructor is used for new instances of TSAttachmentStream
// that represent new, un-uploaded outgoing attachments.
- (instancetype)initAttachmentWithContentType:(NSString *)contentType
                                    byteCount:(UInt32)byteCount
                               sourceFilename:(nullable NSString *)sourceFilename
                                      caption:(nullable NSString *)caption
                               albumMessageId:(nullable NSString *)albumMessageId;

// This constructor is used for new instances of TSAttachmentStream
// that represent downloaded incoming attachments.
- (instancetype)initWithPointer:(TSAttachmentPointer *)pointer transaction:(SDSAnyReadTransaction *)transaction;

// --- CODE GENERATION MARKER

// This snippet is generated by /Scripts/sds_codegen/sds_generate.py. Do not manually edit it, instead run `sds_codegen.sh`.

// clang-format off

- (instancetype)initWithGrdbId:(int64_t)grdbId
                      uniqueId:(NSString *)uniqueId
                  albumMessageId:(nullable NSString *)albumMessageId
         attachmentSchemaVersion:(NSUInteger)attachmentSchemaVersion
                  attachmentType:(TSAttachmentType)attachmentType
                        blurHash:(nullable NSString *)blurHash
                       byteCount:(unsigned int)byteCount
                         caption:(nullable NSString *)caption
                     contentType:(NSString *)contentType
                   encryptionKey:(nullable NSData *)encryptionKey
                    isDownloaded:(BOOL)isDownloaded
                        serverId:(unsigned long long)serverId
                  sourceFilename:(nullable NSString *)sourceFilename
NS_SWIFT_NAME(init(grdbId:uniqueId:albumMessageId:attachmentSchemaVersion:attachmentType:blurHash:byteCount:caption:contentType:encryptionKey:isDownloaded:serverId:sourceFilename:));

// clang-format on

// --- CODE GENERATION MARKER

- (nullable instancetype)initWithCoder:(NSCoder *)coder;

- (void)upgradeFromAttachmentSchemaVersion:(NSUInteger)attachmentSchemaVersion;

@property (nonatomic, readonly) BOOL isAnimated;
@property (nonatomic, readonly) BOOL isImage;
@property (nonatomic, readonly) BOOL isVideo;
@property (nonatomic, readonly) BOOL isAudio;
@property (nonatomic, readonly) BOOL isVoiceMessage;
@property (nonatomic, readonly) BOOL isVisualMedia;
@property (nonatomic, readonly) BOOL isOversizeText;

+ (NSString *)emojiForMimeType:(NSString *)contentType;

#pragma mark - Update With...

- (void)updateWithBlurHash:(NSString *)blurHash transaction:(SDSAnyWriteTransaction *)transaction;

@end

NS_ASSUME_NONNULL_END
