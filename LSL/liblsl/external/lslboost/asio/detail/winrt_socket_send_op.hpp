//
// detail/winrt_socket_send_op.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.lslboost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_DETAIL_WINRT_SOCKET_SEND_OP_HPP
#define BOOST_ASIO_DETAIL_WINRT_SOCKET_SEND_OP_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <lslboost/asio/detail/config.hpp>

#if defined(BOOST_ASIO_WINDOWS_RUNTIME)

#include <lslboost/asio/detail/addressof.hpp>
#include <lslboost/asio/detail/bind_handler.hpp>
#include <lslboost/asio/detail/buffer_sequence_adapter.hpp>
#include <lslboost/asio/detail/fenced_block.hpp>
#include <lslboost/asio/detail/handler_alloc_helpers.hpp>
#include <lslboost/asio/detail/handler_invoke_helpers.hpp>
#include <lslboost/asio/detail/winrt_async_op.hpp>
#include <lslboost/asio/error.hpp>

#include <lslboost/asio/detail/push_options.hpp>

namespace lslboost {
namespace asio {
namespace detail {

template <typename ConstBufferSequence, typename Handler>
class winrt_socket_send_op :
  public winrt_async_op<unsigned int>
{
public:
  BOOST_ASIO_DEFINE_HANDLER_PTR(winrt_socket_send_op);

  winrt_socket_send_op(const ConstBufferSequence& buffers, Handler& handler)
    : winrt_async_op<unsigned int>(&winrt_socket_send_op::do_complete),
      buffers_(buffers),
      handler_(BOOST_ASIO_MOVE_CAST(Handler)(handler))
  {
  }

  static void do_complete(io_service_impl* owner, operation* base,
      const lslboost::system::error_code&, std::size_t)
  {
    // Take ownership of the operation object.
    winrt_socket_send_op* o(static_cast<winrt_socket_send_op*>(base));
    ptr p = { lslboost::asio::detail::addressof(o->handler_), o, o };

    BOOST_ASIO_HANDLER_COMPLETION((o));

#if defined(BOOST_ASIO_ENABLE_BUFFER_DEBUGGING)
    // Check whether buffers are still valid.
    if (owner)
    {
      buffer_sequence_adapter<lslboost::asio::const_buffer,
          ConstBufferSequence>::validate(o->buffers_);
    }
#endif // defined(BOOST_ASIO_ENABLE_BUFFER_DEBUGGING)

    // Make a copy of the handler so that the memory can be deallocated before
    // the upcall is made. Even if we're not about to make an upcall, a
    // sub-object of the handler may be the true owner of the memory associated
    // with the handler. Consequently, a local copy of the handler is required
    // to ensure that any owning sub-object remains valid until after we have
    // deallocated the memory here.
    detail::binder2<Handler, lslboost::system::error_code, std::size_t>
      handler(o->handler_, o->ec_, o->result_);
    p.h = lslboost::asio::detail::addressof(handler.handler_);
    p.reset();

    // Make the upcall if required.
    if (owner)
    {
      fenced_block b(fenced_block::half);
      BOOST_ASIO_HANDLER_INVOCATION_BEGIN((handler.arg1_, handler.arg2_));
      lslboost_asio_handler_invoke_helpers::invoke(handler, handler.handler_);
      BOOST_ASIO_HANDLER_INVOCATION_END;
    }
  }

private:
  ConstBufferSequence buffers_;
  Handler handler_;
};

} // namespace detail
} // namespace asio
} // namespace lslboost

#include <lslboost/asio/detail/pop_options.hpp>

#endif // defined(BOOST_ASIO_WINDOWS_RUNTIME)

#endif // BOOST_ASIO_DETAIL_WINRT_SOCKET_SEND_OP_HPP
