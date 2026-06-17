import * as zod from "zod";

/**
 * Transaction record
 */
export const TransactionResponse = zod.object({
  id: zod.number(),
  accountId: zod.number(),
  transactionType: zod.enum([
    "DEPOSIT",
    "WITHDRAWAL",
    "TRANSFER_OUT",
    "TRANSFER_IN",
    "ACCOUNT_OPENED",
    "ACCOUNT_CLOSED",
    "PIN_CHANGED",
    "INTEREST_APPLIED",
  ]),
  amount: zod.string(),
  balanceBefore: zod.string(),
  balanceAfter: zod.string(),
  description: zod.string().nullable(),
  createdAt: zod.string().datetime(),
});

/**
 * Transaction history response
 */
export const TransactionHistoryResponse = zod.object({
  transactions: zod.array(TransactionResponse),
  total: zod.number(),
});

export type TransactionResponse = zod.infer<typeof TransactionResponse>;
export type TransactionHistoryResponse = zod.infer<typeof TransactionHistoryResponse>;
